#version 430
layout(location = 0) out float fragDepth;

void main()
{
	//Shadowmaps Fragmentshader should not do anything
	fragDepth = gl_FragCoord.z;
}
