
#version 330 core


in vec2 uv;
out vec4 FragColor;

uniform sampler2D sceneTexture;
//uniform float aberration;
uniform float time;
uniform bool isFlicker;
uniform int randomNumber;
uniform float yRange;




float lerp(float a, float b, float t) 
{
    return a + t * (b - a);
}
void main() 
{
    vec2 sampledUv = uv;

   
    float flickerAmplitude = 0.2;
    float flickerOffset = 0.5;
    float flicker = sin(time) * flickerAmplitude + flickerOffset;

   
    sampledUv.y += flicker;

 
    sampledUv.y = mod(sampledUv.y, 1.0);

 
    vec4 screenColor = texture(sceneTexture, isFlicker ? sampledUv : uv);

    FragColor = screenColor;
}

//void mainImage( out vec4 fragColor, in vec2 fragCoord )
//{
//    float t = iTime;
//    t *= UPDATE_RATE;
//    t -= fract(t);
//    t /= UPDATE_RATE;
//    float intensity = (MINIMUM_LIGHT_INTENSITY + (MAXIMUM_LIGHT_INTENSITY + (sin(t) * MAXIMUM_LIGHT_INTENSITY)));
//    intensity += LIGHT_INTENSITY_RANDOMNESS + (hash11(t) * 2.0) - 1.0;
//    intensity /= MAXIMUM_LIGHT;
//	fragColor = vec4( color* intensity,1.0);
//}