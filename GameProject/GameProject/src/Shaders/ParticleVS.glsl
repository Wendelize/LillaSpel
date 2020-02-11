#version 430
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square


out vertex_out{
	vec4 color;
	vec2 texCoords;
} vo;


uniform vec3 u_CameraRight;
uniform vec3 u_CameraUp;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	float particleSize = xyzs.w; 
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ u_CameraRight * squareVertices.x * particleSize
		+ u_CameraUp * squareVertices.y * particleSize;

	gl_Position = u_Proj * u_View * vec4(vertexPosition_worldspace, 1.0f);

	vo.texCoords = squareVertices.xy + vec2(0.5, 0.5);
	vo.color = color;
}