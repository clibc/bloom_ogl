#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint create_gl_array_buffer(const void *ptr, size_t size)
{
    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW);

    return buffer_id;
}

GLuint create_shader(const char *vertex_path, const char *frag_path)
{
    GLuint shader_program = LoadShaders(vertex_path, frag_path);
    return shader_program;
}

bool load_uniform_mat4(GLuint shader_program, const char *name , const float *mvp)
{
	glUseProgram(shader_program);

    bool result;
    GLint matrixID = glGetUniformLocation(shader_program, name);
    result = matrixID != -1;

    if(!result) {
        fprintf(stderr, "failed to load uniform '%s'\n", name);
        return result;
    }

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, mvp);
    
    return result;
}

bool load_uniform_vec3(GLuint shader_program, const char *name , const float *vector)
{
	glUseProgram(shader_program);

    bool result;
    GLint matrixID = glGetUniformLocation(shader_program, name);
    result = matrixID != -1;

    if(!result) {
        fprintf(stderr, "failed to load uniform '%s'\n", name);
        return result;
    }

    glUniform3fv(matrixID, 1, vector);
    
    return result;
}

unsigned char* create_texture_rgb(const char *texture_path, int *width, int *height, int *channels)
{
    unsigned char* texture_data = NULL;
    stbi_set_flip_vertically_on_load(true);
    texture_data = stbi_load(texture_path, width, height, channels, 0);
    if(texture_data == NULL){
        fprintf(stderr, "Failed to load texture");
        exit(-1);
    }
    fprintf(stdout, "Texture loaded... Width: %d, Height: %d", *width, *height);

    GLuint textureID;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    return texture_data;
}
