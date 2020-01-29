#version 430
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_UV;

out vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec3 tex_coords;
} vo;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	vo.tex_coords = a_Position;
	vec4 pos	  = u_Projection * u_View * vec4(a_Position, 1);
	gl_Position	  = pos.xyww;
	vo.normal	  = normalize(a_Normal);
	vo.color	  = a_Color;
	
} 