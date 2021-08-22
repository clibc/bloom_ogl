#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 textureCoordIn;

uniform mat4 mvp;
uniform mat4 just_model;
uniform vec3 light_position;

out vec2 textureCoord;

out vec3 diffuse;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);
    textureCoord = textureCoordIn;

    vec4 light_model_pos = just_model * vec4(light_position, 1.0);
    vec3 light_dir = normalize(light_model_pos.xyz - vertexPosition);
    vec3 light_color = vec3(0,1,0);
    float diff = max(dot(vertexColor, light_dir), 0.1); // vertex color is vertex normal for real
    diffuse = diff * light_color;
}
