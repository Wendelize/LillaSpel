#version 430

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec4 Position;

uniform vec3 u_ApexColor;
uniform vec3 u_CenterColor;

void main()
{
	float height = Position.y + 1;

	if(height > 1.0f)
	{
		FragColor = mix(vec4(u_ApexColor, 1), vec4(0, 0, 0, 1), (height - 1) * 8);
	}
	else
	{
		FragColor = mix(vec4(u_CenterColor, 1), vec4(u_ApexColor, 1), height);
	}
}
