#version 330 core

in vec3 fragmentColor;
in vec2 textureCoord;

out vec4 color;

uniform sampler2D texture;

void main()
{
	color = texture(texture, textureCoord);
}