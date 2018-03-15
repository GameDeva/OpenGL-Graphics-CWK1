#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D boxTexture;

void main()
{
	// fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	fragColor = texture(boxTexture, texCoord); 
}