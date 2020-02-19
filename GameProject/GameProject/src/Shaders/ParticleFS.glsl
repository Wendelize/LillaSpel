#version 430
// Ouput data
out vec4 FragColor;

in vertex_out{
	vec4 color;

} vi;

uniform sampler2D u_Tex;

void main()
{
	FragColor = vi.color; 
}