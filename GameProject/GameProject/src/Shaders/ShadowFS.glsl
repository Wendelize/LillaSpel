#version 430
//out float fragDepth;

in vertex_out{
	vec3 position;
	vec4 positionLightSpace;
} vi;



void main()
{
	//Shadowmaps Fragmentshader should not do anything
	//fragDepth = gl_FragCoord.z;
	//fragDepth = vec4(vi.positionLightSpace);
}
