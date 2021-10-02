#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordIn;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(vertexPosition.xyz, 1.0);
    TexCoords = textureCoordIn;
}
