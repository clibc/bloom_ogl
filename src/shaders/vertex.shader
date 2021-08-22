#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 textureCoordIn;

uniform mat4 mvp;
uniform mat4 model;

out vec2 textureCoord;
out vec3 normal;
out vec3 frag_pos;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);

    frag_pos = vec3(model * vec4(vertexPosition, 1.0));
    frag_pos = vertexPosition;
    textureCoord = textureCoordIn;
    normal = vec3(model * vec4(vertex_normal, 1.0));
}

// so I transformed vertex position and normals into world space
// basically via multipying them by model matrix of the object
// I do not transform light position because by convention I assume light is already in
// world coordinates
// so they need to be in the same coordinate space system in order to get nice looking light
