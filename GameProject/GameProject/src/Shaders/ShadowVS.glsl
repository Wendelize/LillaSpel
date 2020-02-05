#version 430
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_UV;

uniform mat4 u_LSP;
uniform mat4 u_Model;

out vertex_out{
	vec3 position;
	vec4 positionLightSpace;
} vo;

void main()
{
	vo.position = vec3(u_Model * vec4(a_Position, 1.0));
	vo.positionLightSpace = u_LSP * u_Model * vec4(a_Position, 1.0);
	gl_Position = vo.positionLightSpace;
} 