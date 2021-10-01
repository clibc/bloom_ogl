#version 330 core

in vec2 textureCoord;
in vec3 normal;
in vec3 frag_pos;
in vec3 light_pos;

uniform sampler2D texture;
uniform vec3 light_position;

out vec4 color;

void main()
{
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - frag_pos);

    float diffuse = max(dot(norm, light_direction), 0.05);

    vec3 trashold = vec3(0.7126, 0.71152,  0.0722);

    color = texture2D(texture, textureCoord) * diffuse;

    float brightness = dot(color.rgb, trashold);

    //color = vec4(1,0,0,1.0f);

    if(brightness > 1.0)
    {
        color = vec4(color.rgb, 1.0f);
    }
    else {
        color = vec4(0, 0, 0, 1.0f);
    }
}
