
#version 330 core

const int 	fps 	= 15;
const float delta 	= 1.0f / float(fps);

in vec2 uv;
out vec4 FragColor;

uniform sampler2D sceneTexture;
//uniform sampler2D staticTexture;
uniform float time;
uniform bool isEffectActive;


void main() 
{
  
    vec4 originalColor = texture(sceneTexture, uv);

  
    float randomNoise = fract(sin(dot(uv + time, vec2(12.9898, 78.233))) * 43758.5453);

 
    float staticIntensity = 0.5;

    
   
    vec4 finalColor =  !isEffectActive ? originalColor : mix(originalColor, vec4(vec3(randomNoise), 1.0), 
                                                                                       staticIntensity);

    FragColor = finalColor;
}
