#version 430
#define MAX_NR_OF_LIGHTS 40

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

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
uniform int u_NrOfCarLights;
uniform vec3 u_ViewPos;
uniform int u_LightType;
uniform Light u_Lights[MAX_NR_OF_LIGHTS];
uniform Light u_CarLights[MAX_NR_OF_LIGHTS];
uniform Material u_Material;
uniform bool u_Glow;
uniform bool u_LightsOut;

uniform sampler2D u_ShadowMap;


//--------------------------------------------------------------------------------------------------
// FUNCTIONS

// SHADOWS (only from driectional-light)
float CalcShadow(Light light, vec4 positionLightSpace, vec3 p, vec3 n, vec3 eye)
{
    vec3 projCoords = (positionLightSpace.xyz / positionLightSpace.w) * 0.5 + 0.5;
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float bias =  max(0.5 * (1.0 - dot(n, -light.dir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;
        
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
	vec3 lightVec = normalize(-light.dir);
	vec3 lookVec = normalize(eye - p);

	vec3 ambient = light.ambient * light.color * u_Material.ambient * vi.color;
	vec3 diffuse = CalcDiffuse(light, lightVec, n) * u_Material.diffuse * vi.color;
	vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn) * u_Material.specular;

    if(!shadow) {
	    return (ambient + diffuse + specular) * light.color;
    }
    else if(shadow) {
        float shadow = CalcShadow(light, vi.positionLightSpace, p, n, eye);
        return (ambient + (0.5 - shadow) * (diffuse + specular)) * light.color;
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
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.092 * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * light.color;
}


// SPOTLIGHT
vec3 CalcSpotLight(Light light, vec3 p, vec3 n, vec3 eye, bool blinn) {
	vec3 lightVec = normalize(light.pos - p);
	float theta = dot(lightVec, normalize(-light.dir));
	vec3 result = vec3(0);
	float lightLength = length(light.pos - p);

	if(lightLength < 10)
	{
		if(theta > light.cutOff)
		{
			vec3 col = normalize(vi.color);
			vec3 lookVec = normalize(eye - p);

			vec3 ambient = light.ambient * u_Material.ambient * col;
		
			vec3 diffuse = CalcDiffuse(light, lightVec, n) * u_Material.diffuse * col;

			vec3 specular = CalcSpecular(light, lightVec, lookVec, n, blinn) * u_Material.specular;

			float distance = length(light.pos - p);
			float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.12 * (distance * distance));    

			diffuse *= attenuation;
			specular *= attenuation;

			result = ( diffuse + specular) ;
		}
		else
			{
				result = vec3(0);
			}
	}
	else
	{
		result = vec3(0);
	}
	return result;
}


// MAIN
void main(){
	vec3 result = vec3(0.0);
	bool blinn = true;
	vec3 col = normalize(vi.color);
	vec3 ambient = u_Material.ambient * col * 0.05;
	
	for(int i = 0; i < u_NrOf; i++)
	{
		if(u_Lights[i].type == 0)
		{
			if(u_LightsOut== false)
			result += CalcDirLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, true, true);

		} else if (u_Lights[i].type == 1)
		{
			result += CalcPointLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, blinn); 
		} else if (u_Lights[i].type == 2) 
		{
			result += CalcSpotLight(u_Lights[i], vi.position, vi.normal, u_ViewPos, false); 
		}
	}
	

    FragColor = vec4(result + ambient, 1.0);

    float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(u_Glow)// && brightness > 1)
        BrightColor = vec4(result.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}
