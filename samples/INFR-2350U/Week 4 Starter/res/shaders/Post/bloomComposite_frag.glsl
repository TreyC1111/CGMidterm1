#version 420

layout(binding = 0) uniform sampler2D uScene;
layout(binding = 1) uniform sampler2D uBloom;

in vec2 TexCoords;
out vec4 fragColor;



void main() 
{
	vec4 colorOne = texture(uScene, TexCoords);
	vec4 colorTwo = texture(uBloom, TexCoords);
	fragColor = 1.0 - (1.0 - colorOne) * (1.0 - colorTwo);
}