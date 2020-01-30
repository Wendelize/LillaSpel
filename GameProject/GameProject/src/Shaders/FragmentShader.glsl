#version 430

out vec4 fragmentColor;

in vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec2 tex_coords;
} vi;

in vec3 view;

struct DirLight{
	vec3 dir, color;
	float ambient, diffuse, specular;
};

struct SpotLight {
	vec3 pos, color;
	float ambient, diffuse, specular;
	float constant, linear, quadratic;
	float cutOff, outerCutOff;
};

struct PointLight {
	vec3 pos, color;
	float constant, linear, quadratic;
	float ambient, diffuse, specular;
};
//maybe send in a nrOfLights
const int NR_OF_POINTLIGHTS = 2;

uniform DirLight u_DirLight;
uniform SpotLight u_SpotLight;
uniform PointLight u_PointLight[NR_OF_POINTLIGHTS];

// DIRECTIONAL-LIGHT
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shininess) {
	vec3 lightDir = normalize(-light.dir);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = light.ambient * light.color;
	vec3 diffuse = light.diffuse * diff * light.color;
	vec3 specular = light.specular * spec * light.color;

	return (ambient + diffuse + specular) * light.color;
}

// POINT-LIGHT
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float alpha)
{
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), alpha);

    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    

    vec3 ambient = light.ambient * light.color;
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// SPOT-LIGHT
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float alpha)
{
    vec3 lightDir = normalize(light.pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), alpha);
    // attenuation
    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize((vec3(0, 5, -5)-vec3(0)))); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * light.color;
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 specular = light.specular * spec * light.color;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

// TEST CALC OF LIGHT
vec3 BasicLight(SpotLight light, float alpha, vec3 normal, vec3 p){
	vec3 lightVec = normalize(light.pos - p);
	vec3 lookVec = normalize(vec3(0, 5, -10) - p);
	vec3 reflectVec = normalize(reflect(-lightVec, normal));

	float dotDiff = dot(lightVec, normal);
	float dotSpec = dot(reflectVec, lookVec);

	if(dotDiff < 0.0)
		return light.ambient * light.color;

	if(dotSpec < 0.0)
		return light.color * dotDiff * light.diffuse;


	return light.color * (light.diffuse * dotDiff + light.specular * pow(dotSpec, alpha));
}



void main(){
	vec3 result;
	vec3 viewDir = normalize(vec3(0, 5, -10) - vi.position);
	//result += CalcDirLight(u_DirLight, vi.normal, viewDir, 0.0 );
	result += BasicLight(u_SpotLight, 5.0, vi.normal, vi.position);
	for(int i = 0; i < NR_OF_POINTLIGHTS; i++) {
		result += CalcPointLight(u_PointLight[i], vi.normal, vi.position, viewDir, 5.0); 
	}
	result += CalcSpotLight(u_SpotLight, vi.normal, vi.position, viewDir, 10.0);

	fragmentColor = vec4(result * vec3(1), 1.0f);
}
