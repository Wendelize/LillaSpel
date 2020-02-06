#version 430
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_UV;

uniform mat4 u_LSP;
uniform mat4 u_Model;


void main()
{
	gl_Position = u_LSP * u_Model * vec4(a_Position, 1.0);
} 