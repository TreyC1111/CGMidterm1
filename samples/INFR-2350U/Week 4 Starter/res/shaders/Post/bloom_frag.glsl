#version 420

layout(binding = 0) uniform sampler2D uTex;

uniform float u_threshHold;

out vec4 fragColor;

in vec2 TexCoords;

void main() 
{
	vec4 color = texture(uTex, TexCoords);

	float lum = (color.r + color.g + color.b) / 3.0;

	if (lum > u_threshHold)
	{
		fragColor = color;
	}
	else
	{
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}