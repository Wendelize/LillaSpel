#version 430

out vec4 fragmentColor;

in vertex_out{
	vec3 position;
	vec3 normal;
	vec3 color;
} _in;

void main(){
	fragmentColor = vec4(_in.color, 1.0f);
}
