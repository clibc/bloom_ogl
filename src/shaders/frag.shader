#version 330 core

in vec2 textureCoord;
in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos;

uniform sampler2D texture;
uniform vec3 light_position;

//out vec4 color;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 bright_color;

void main()
{
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - frag_pos);

    float diffuse = max(dot(norm, light_direction), 0.05);
    color = texture2D(texture, textureCoord) * diffuse;

    vec3 trashold = vec3(0.7126, 0.71152,  0.0722);
    float brightness = dot(color.rgb, trashold);

    if(brightness > 1.0)
    {
        bright_color = vec4(color.rgb, 1.0f);
    }
    else {
        bright_color = vec4(1, 0, 0, 1.0f);
    }
}
