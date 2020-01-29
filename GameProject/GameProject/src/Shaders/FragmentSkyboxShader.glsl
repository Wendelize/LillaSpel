#version 430

out vec4 fragmentColor;

in vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec3 tex_coords;
} vi;

uniform samplerCube u_texture;

void main(){
	fragmentColor = texture(u_texture, vi.tex_coords);
}
