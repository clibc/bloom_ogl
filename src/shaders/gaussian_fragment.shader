#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 color;

void main()
{ 
    color = texture(screenTexture, TexCoords);
}
