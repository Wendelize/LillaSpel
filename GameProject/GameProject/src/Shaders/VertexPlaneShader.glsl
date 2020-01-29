#version 430
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_UV;

out vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec2 tex_coords;
} vo;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position	  = u_Projection * u_View * u_Model * vec4(a_Position, 1);
	vo.normal	  = normalize(a_Normal);
	vo.color	  = a_Color;
	vo.tex_coords = a_UV;
} 