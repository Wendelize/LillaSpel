#version 430
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 color; // Position of the center of the particule and size of the square


out vertex_out{
	vec4 color;
	vec2 texCoords;
} vo;


uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	vec3 u_CameraRight = vec3(u_View[0][0], u_View[1][0], u_View[2][0]);
	vec3 u_CameraUp	   = vec3(u_View[0][1], u_View[1][1], u_View[2][1]);

	float particleSize = xyzs.w; 
	vec3 particleCenter_worldspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_worldspace
		+ u_CameraRight * squareVertices.x * particleSize
		+ u_CameraUp * squareVertices.y * particleSize;

	gl_Position = u_Proj * u_View * vec4(squareVertices, 1.0f);

	vo.texCoords = squareVertices.xy + vec2(0.5, 0.5);
	vo.color = u_Proj * u_View * vec4(squareVertices, 1.0f);
}