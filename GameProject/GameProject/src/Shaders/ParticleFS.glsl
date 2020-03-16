#version 430

// Ouput data

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vertex_out{
	vec4 color;

} vi;

uniform sampler2D u_Tex;

void main()
{
	FragColor = vi.color; 
	BrightColor = vi.color; 
}