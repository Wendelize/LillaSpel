#version 430

out vec4 fragmentColor;

in vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
	vec2 tex_coords;
} vi;

void main(){
	fragmentColor = vec4(vi.normal, 1.0f);
}
