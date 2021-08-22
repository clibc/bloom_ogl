#pragma once

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
    GLuint matrixID = glGetUniformLocation(shader_program, name);
    result = matrixID != -1;

    if(!result) return result;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, mvp);
    
    return result;
}


bool load_uniform_vec3(GLuint shader_program, const char *name , const float *vector)
{
	glUseProgram(shader_program);

    bool result;
    GLuint matrixID = glGetUniformLocation(shader_program, name);
    result = matrixID != -1;

    if(!result) return result;

    glUniform3fv(matrixID, 1, vector);
    
    return result;
}
