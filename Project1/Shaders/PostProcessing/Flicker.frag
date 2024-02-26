
#version 330 core


in vec2 uv;
out vec4 FragColor;

uniform sampler2D sceneTexture;
//uniform float aberration;
uniform float time;
uniform bool isFlicker;
uniform int randomNumber;
uniform float yRange;


float speed = 0.1; 
float frequency = 0.5; 
float amplitude = 0.2;

float lerp(float a, float b, float t) 
{
    return a + t * (b - a);
}
void main() 
{
  


      float flicker = amplitude * sin(2.0 * 3.14159 * frequency * time) + speed * time;

      if(randomNumber<=0)
      {
       flicker*=-1;
      }
   
     vec2 sampledUv = vec2(uv.x, uv.y + flicker);

 
   

 
    vec4 screenColor = texture(sceneTexture, isFlicker ? sampledUv : uv);

    FragColor = screenColor;
}

