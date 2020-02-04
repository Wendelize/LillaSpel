#version 430
#define MAX_NR_OF_LIGHTS 40

out vec4 fragmentColor;

// IN-DATA FROM VERTEXSHADER
in vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec2 tex_coords;
} vi;


// LIGHT DATA
struct Light{
	int type;
	vec3 pos, dir, color;
	float ambient, diffuse, specular;
    float cutOff, outerCutOff;
};


// UNIFORMS
uniform int u_NrOf;
uniform vec3 u_ViewPos;
uniform int u_LightType;
uniform Light u_Lights[MAX_NR_OF_LIGHTS];


// DIFFUSE
vec3 CalcDiffuse(Light light, vec3 lightVec, vec3 n){
	float diff = max(dot(lightVec, n), 0.0);
	vec3 diffuse = diff * light.color * light.diffuse;

	return diffuse;
}


// SPECULAR
vec3 CalcSpecular(Light light, vec3 lightVec, vec3 lookVec, vec3 n, bool blinn){
	float spec = 0.0;

	// BLINN
	if(blinn) {
		vec3 halfwayDir = normalize(lightVec + lookVec);
		spec = pow(max(dot(n, halfwayDir), 0.0), 32.0);
	} 
	// PHONG
	else if (!blinn) {
		vec3 reflectionVec = reflect(-lightVec, n);
		spec = pow(max(dot(lookVec, reflectionVec), 0.0), 10.0);
	}

	vec3 specular = vec3(light.specular) * spec;

	return specular;
}


// DIRLIGHT
vec3 CalcDirLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn){
	vec3 lightVec = normalize(-light.dir);
	vec3 lookVec = normalize(eye - p);

	vec3 ambient = light.ambient * light.color * vi.color;

	vec3 diffuse = CalcDiffuse(light, lightVec, n) * vi.color;

	vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn);


	return (ambient + diffuse + specular) * light.color;
}


// POINTLIGHT
vec3 CalcPointLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn){
    vec3 lightVec = normalize(light.pos - p);
	vec3 lookVec = normalize(eye - p);

	vec3 ambient = light.ambient * light.color * vi.color;

	vec3 diffuse = CalcDiffuse(light, lightVec, n) * vi.color;

    vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn);

    float distance = length(light.pos - p);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}


// SPOTLIGHT
vec3 CalcSpotLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn){
	vec3 lightVec = normalize(light.pos - p);
    float theta = dot(lightVec, normalize(light.dir)); 
    vec3 lookVec = normalize(eye - p);

    if(theta > light.cutOff) {
        vec3 ambient = light.ambient * light.color * vi.color;

        vec3 diffuse = CalcDiffuse(light, lightVec, n) * vi.color;

        vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn);

        float distance = length(light.pos - p);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    
    
        // spotlight intensity
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        ambient *= attenuation * intensity;
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
    }
    else {
        return (light.ambient * light.color);
    }
}

void main(){
	vec3 result = vec3(0.0);
	bool blinn = true;
    vec3 look = normalize(u_ViewPos - vi.position);

	for(int i = 0; i < u_NrOf; i++)
	{
		if(u_Lights[i].type == 0) {
			result += CalcDirLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, false);
		} else if (u_Lights[i].type == 1) {
			result += CalcPointLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, blinn);
		} else if (u_Lights[i].type == 2) {
			//result += CalcSpotLight2(u_Lights[i], vi.normal, vi.position, look);
			result += CalcSpotLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, false);
		}
	}
    
	fragmentColor = vec4(result , 1.0f);
}
