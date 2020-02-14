#version 430
// Ouput data
out vec4 FragColor;

in vertex_out{
	vec4 color;
	//vec2 texCoords;
} vi;

uniform sampler2D u_Tex;

void main(){
	// Output color = color of the texture at the specified UV
	FragColor = vi.color; //texture( u_Tex, vi.texCoords ) * vi.color;

}