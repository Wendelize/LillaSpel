#version 430
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Scene;
uniform sampler2D u_BloomBlur;
uniform sampler2D u_Color;
uniform bool u_Bloom;
uniform float u_Exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 color = texture(u_Scene, TexCoords).rgb;      
    vec3 bloomColor = texture(u_BloomBlur, TexCoords).rgb;
    vec3 aa = texture(u_Color, TexCoords).rgb;
    //if(u_Bloom)
       // color += bloomColor; 
   
  // color += aa;
   // Use for crazy
    vec3 result = vec3(1.0) - exp(-color * u_Exposure);
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(color, 1.0);
}