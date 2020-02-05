#version 430
#define MAX_NR_OF_LIGHTS 40

out vec4 fragmentColor;

// IN-DATA FROM VERTEXSHADER
in vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec2 texCoords;
	vec4 positionLightSpace;
} vi;


//--------------------------------------------------------------------------------------------------


// LIGHT STRUCT
struct Light{
	int type;
	vec3 pos, dir, color;
	float ambient, diffuse, specular;
    float cutOff, outerCutOff;
};

// MATERIAL STRUCT
struct Material {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float shininess;
};

// UNIFORMS
uniform int u_NrOf;
uniform vec3 u_ViewPos;
uniform int u_LightType;
uniform Light u_Lights[MAX_NR_OF_LIGHTS];
uniform Material u_Material;

uniform sampler2D u_ShadowMap;


//--------------------------------------------------------------------------------------------------
// FUNCTIONS

// SHADOWS (only from driectional-light)
float CalcShadow(Light light, vec4 positionLightSpace, vec3 p, vec3 n, vec3 eye)
{
    vec3 projCoords = (positionLightSpace.xyz / positionLightSpace.w) * 0.5 + 0.5;
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(light.pos - p);
    float bias = max(0.05 * (1.0 - dot(n, lightDir)), 0.005);
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
//    for(int x = -1; x <= 1; ++x)
//    {
//        for(int y = -1; y <= 1; ++y)
//        {
//            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
//            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
//        }    
//    }
//    shadow /= 9.0;
//
//    if(projCoords.z > 1.0)
//        shadow = 0.0;
        
    return shadow;
}

// DIFFUSE
vec3 CalcDiffuse(Light light, vec3 lightVec, vec3 n){
	float diff = max(dot(n, lightVec), 0.0);
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
		spec = pow(max(dot(lookVec, reflectionVec), 0.0), u_Material.shininess);
	}
	vec3 specular = vec3(light.specular) * spec;
	return specular;
}


// DIRLIGHT
vec3 CalcDirLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn, bool shadow){
	vec3 lightVec = normalize(-light.dir - p);
	vec3 lookVec = normalize(eye - p);

	vec3 ambient = light.ambient * light.color * u_Material.ambient * vi.color;
	vec3 diffuse = CalcDiffuse(light, lightVec, n) * u_Material.diffuse * vi.color;
	vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn) * u_Material.specular;

    if(!shadow) {
	    return (ambient + diffuse + specular) * light.color;
    }
    else if(shadow) {
        float shadow = CalcShadow(light, vi.positionLightSpace, p, n, eye);
        return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.color;
    }
}


// POINTLIGHT
vec3 CalcPointLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn){
    vec3 lightVec = normalize(light.pos - p);
	vec3 lookVec = normalize(eye - p);

	vec3 ambient = light.ambient * light.color * u_Material.ambient * vi.color;
	vec3 diffuse = CalcDiffuse(light, lightVec, n) * u_Material.diffuse * vi.color;
    vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn) * u_Material.specular;

    float distance = length(light.pos - p);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}


// SPOTLIGHT pls do not use 
vec3 CalcSpotLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn){
	vec3 lightVec = normalize(light.pos - p);
    float theta = dot(lightVec, normalize(-light.dir)); 
    vec3 lookVec = normalize(eye - p);

    vec3 ambient = light.ambient * light.color * u_Material.ambient * vi.color;
    vec3 diffuse = CalcDiffuse(light, lightVec, n) * u_Material.diffuse * vi.color;
    vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn) * u_Material.specular;

    float distance = length(light.pos - p);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    
    
    // spotlight intensity
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular) * light.color;
  
}
// SPOTLIGHT pls do not use 
vec3 CalcSpotLightV3(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, bool blinn)
{
    vec3 lightDir = normalize(light.pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.dir)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * light.color * u_Material.ambient * vi.color;
    vec3 diffuse = /*light.diffuse*/ CalcDiffuse(light, lightDir, normal) * u_Material.diffuse * vi.color;
    vec3 specular = light.specular/*CalcSpecular(light, lightDir, viewDir, normal, false)*/  * spec * u_Material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular) * light.color;
}

// MAIN
void main(){
	vec3 result = vec3(0.0);
	bool blinn = true;

	for(int i = 0; i < u_NrOf; i++)
	{
		if(u_Lights[i].type == 0) {
			result += CalcDirLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, true, false);
		} else if (u_Lights[i].type == 1) {
			result += CalcPointLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, blinn);
		} //else if (u_Lights[i].type == 2) {
			//result += CalcSpotLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, blinn);
            //result += CalcSpotLightV3(u_Lights[i], vi.normal, vi.position, look, blinn);
		//}
	}
    float c = CalcShadow(u_Lights[0], vi.positionLightSpace, vi.position, vi.normal, u_ViewPos);
    vec3 m = vi.positionLightSpace.xyz;
//  fragmentColor = vec4(vec3(texture(u_ShadowMap, vi.texCoords).x), 1.0);
//	fragmentColor = vec4(m, 1.0);
    fragmentColor = vec4(gl_FragCoord);
}
