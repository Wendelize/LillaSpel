#version 430
layout(location = 0) in vec3 a_Position;

out vec3 tex_coords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	tex_coords	  = a_Position;
	vec4 pos	  = u_Projection * u_View * vec4(a_Position, 1);
	gl_Position	  = pos.xyww;
} 