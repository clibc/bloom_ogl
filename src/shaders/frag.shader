#version 330 core

in vec2 textureCoord;
in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos;

uniform sampler2D texture;

out vec4 color;

void main()
{
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_pos - frag_pos);

    float diffuse = max(dot(norm, light_direction), 0.05);

	color = texture(texture, textureCoord) * diffuse;
}
