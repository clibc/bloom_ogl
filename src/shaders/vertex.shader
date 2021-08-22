#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 textureCoordIn;

uniform mat4 mvp;
uniform mat4 model;
uniform vec3 light_position;

out vec2 textureCoord;
out vec3 normal;
out vec3 frag_pos;
out vec3 light_pos;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);

    frag_pos = vec3(model * vec4(vertexPosition, 1.0));
    frag_pos = vertexPosition;
    textureCoord = textureCoordIn;
    normal = vertex_normal;
    light_pos = vec3(inverse(model) * vec4(light_position, 1.0)); // inverse matrix
}
