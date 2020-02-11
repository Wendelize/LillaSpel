#version 430

out vec4 fragmentColor;

in vec3 tex_coords;

uniform samplerCube u_texture;

void main(){
	fragmentColor = texture(u_texture, tex_coords);
}
