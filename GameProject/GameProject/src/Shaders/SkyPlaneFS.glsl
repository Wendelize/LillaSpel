#version 430

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 UV;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;

uniform float u_firstTranslationX;
uniform float u_firstTranslationZ;
uniform float u_secondTranslationX;
uniform float u_secondTranslationZ;

void main()
{
	vec3 temp = texture2D(u_Texture1, (UV.xy * 7 + vec2(u_firstTranslationX, u_firstTranslationZ))).xyz * 1.65;

	vec4 overLayer = vec4(temp, dot(temp, vec3(0.20)));

	temp = texture2D(u_Texture2, (UV.xy * 3 + vec2(u_secondTranslationX, u_secondTranslationZ))).xyz * 1.5;

	vec4 underLayer = vec4(temp, dot(temp, vec3(0.20)));

	vec4 clouds = overLayer * underLayer;

	vec3 earth = texture2D(u_Texture3, UV).xyz;

	FragColor = vec4(0, 0, 0, clouds.w);//vec4(mix(earth, clouds.xyz, clouds.w), 1);
}