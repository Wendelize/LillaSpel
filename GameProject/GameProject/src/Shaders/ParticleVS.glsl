#version 430
layout(location = 0) in vec3 a_Quad;
layout(location = 1) in vec4 a_Position; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 a_Color; 

out vertex_out
{
	vec4 color;
} vo;

uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	vec3 u_CameraRight = vec3(u_View[0][0], u_View[1][0], u_View[2][0]);
	vec3 u_CameraUp	   = vec3(u_View[0][1], u_View[1][1], u_View[2][1]);

	float particleSize = a_Position.w; 
	vec3 particleCenter_worldspace = a_Position.xyz;
	vec3 vert = a_Quad * particleSize;

	vec3 vertexPosition_worldspace = 
		particleCenter_worldspace
		+ u_CameraRight * a_Quad.x * particleSize
		+ u_CameraUp * a_Quad.y * particleSize;

	gl_Position = u_Proj * u_View * vec4(vertexPosition_worldspace, 1.0f);
	vo.color = a_Color;
}