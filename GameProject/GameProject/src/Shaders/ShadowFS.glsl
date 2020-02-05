#version 430
out float fragDepth;

void main()
{
	//Shadowmaps Fragmentshader should not do anything
	fragDepth = gl_FragCoord.z;
}
