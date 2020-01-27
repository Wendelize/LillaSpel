#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

out vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
} _out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position	  = projection * view * model * vec4(position, 1);
	_out.normal   = normalize(normal);
	_out.color	  = color;
} 