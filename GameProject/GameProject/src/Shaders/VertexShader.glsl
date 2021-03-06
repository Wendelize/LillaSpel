#version 430
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_UV;

out vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec2 texCoords;
	vec4 positionLightSpace;
} vo;


uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_LSP;

uniform vec3 u_PlayerColor;


void main()
{
	vo.position   = vec3(u_Model * vec4(a_Position, 1.0));
	gl_Position	  = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	vo.normal	  = normalize(vec3(u_Model * vec4(a_Normal, 0.0)));
	vo.color	  = u_PlayerColor;
	vo.texCoords = a_UV;
	vo.positionLightSpace = u_LSP * vec4(vo.position, 1.0);
} 