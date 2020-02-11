#version 430
layout (location = 0)in vec2 a_Position;

out vec4 particleColor;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec4 u_Color;
uniform vec2 u_Offset;

void main()
{
	particleColor = vec4(u_Color, 1.0);
	vec4 pos	  = u_Projection * u_View * vec4((vec2(4,4) + u_Offset), 0.0, 1.0);
	gl_Position	  = pos;
} 