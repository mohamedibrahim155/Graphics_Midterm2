
#version 330 core

const int 	fps 	= 15;
const float delta 	= 1.0f / float(fps);

in vec2 uv;
out vec4 FragColor;

uniform sampler2D sceneTexture;
//uniform sampler2D staticTexture;
uniform float time;



void main() 
{
  
    vec4 originalColor = texture(sceneTexture, uv);

  
    float randomNoise = fract(sin(dot(uv + time, vec2(12.9898, 78.233))) * 43758.5453);

 
    float staticIntensity = 0.5;

   
    vec4 finalColor = mix(originalColor, vec4(vec3(randomNoise), 1.0), staticIntensity);
    FragColor = finalColor;
}
//void main() 
//{
//    // Assuming 'uv' is the texture coordinates and 'sceneTexture' is the texture you're sampling from
//  
//
//    // Introduce random noise
//   ; // Adjust the intensity of the static effect
//    float randomNoise = (fract(sin(dot(gl_FragCoord.xyz, vec3(12.9898, 78.233, 45.5432))) * 43758.5453) - 0.5) * 2.0;
//
//    // Mix the original color with the static effect
//    vec4 finalColor = mix(originalColor, vec4(vec3(randomNoise * staticIntensity), 1.0), staticIntensity);
//
//    // Output the final color
//    FragColor = finalColor;
//}