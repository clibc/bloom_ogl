#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;

out vec4 color;

void main()
{
    const float gamma = 2.2;
    const float exposure = 1.0;
    
    vec3 sColor = texture(screenTexture, TexCoords).rgb;
    vec3 bColor = texture(blurTexture, TexCoords).rgb;

    sColor += bColor;

    vec3 result = vec3(1.0) - exp(-sColor * exposure);
    result = pow(result, vec3(1.0 / gamma));

    color = vec4(result, 1.0);
}
