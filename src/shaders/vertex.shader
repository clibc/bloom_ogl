#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 textureCoordIn;

uniform mat4 mvp;
uniform vec3 light_position;

out float dot_result;
out vec2 textureCoord;

out vec3 diffuse;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);
    textureCoord = textureCoordIn;

    vec3 light_dir = normalize(light_position - vertexPosition);
    vec3 light_color = vec3(0,1,0);
    float diff = max(dot(vertexColor, light_dir), 0.1); // vertex color is vertex normal for real
    diffuse = diff * light_color;
}
