#include <iostream>
#include "glew.h"
#include "glfw3.h"
#include "shader.hpp"

#include "mat4x4.hpp"
#include "vec3.hpp"
#include "gtc/matrix_transform.hpp"

#include "cube.h"
#include "objloader.hpp"
#include "openglutils.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 800
#define HEIGHT 600

void handle_window_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static glm::mat4 model_matrix_edit = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, -2.0f));

int main(int argc, char *argv[])
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texturecoords;
	std::vector<glm::vec3> normals;
	bool load_result = load_obj_file_basic("assets/sphere.obj",
						vertices,
						texturecoords,
						normals);

	if(!load_result) { exit(-1); }

	GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "loser engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, handle_window_input);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	// create cube buffers

	// buffers
	GLuint cube_vb = create_gl_array_buffer(&vertices[0], vertices.size() * sizeof(glm::vec3));
	GLuint cube_color_vb = create_gl_array_buffer(&normals[0], normals.size() * sizeof(glm::vec3));
    GLuint cube_texture_vb = create_gl_array_buffer(&texturecoords[0], texturecoords.size() * sizeof(glm::vec2));
	///

    GLuint shaderProgram = create_shader("./src/shaders/vertex.shader", "./src/shaders/frag.shader");

	// MATRICES STUFF
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f),
												   4.0f / 3.0f,
												   0.1f,
												   100.0f);
	
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(5,4,6),
                                        glm::vec3(0,0,0),
                                        glm::vec3(0,1,0));
    glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::mat4 mvp_matrix = projection_matrix * view_matrix * model_matrix;
    glm::vec3 light_position = glm::vec3(4.0f, 4.0f, 4.0f);

    load_uniform_mat4(shaderProgram, "mvp", &mvp_matrix[0][0]);
    load_uniform_vec3(shaderProgram, "light_position", &light_position.x);

    // create texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned int texture = create_texture_rgb("assets/freebsdsmash.jpg", &width, &height, &channels);
    
    // create frame buffer object, texture to render etc ...
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);


    // fbo textures array
    GLuint fbo_textures[2];
    
    glGenTextures(1, &fbo_textures[0]);
    glBindTexture(GL_TEXTURE_2D, fbo_textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_textures[0], 0);
    
    glGenTextures(1, &fbo_textures[1]);
    glBindTexture(GL_TEXTURE_2D, fbo_textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbo_textures[1], 0);

    unsigned int attachmets[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachmets);
    ///

    GLuint fbo_depth_texture;
    glGenTextures(1, &fbo_depth_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_texture, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
        fprintf(stdout, "Framebuffer is complete\n");
    }
    else {fprintf(stdout, "Framebuffer is not complete\n");}    
    //

    
    /// pin-pong buffers && shaders
    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffer[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL
            );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
            );
    }

    GLuint quad_shader = create_shader("./src/shaders/gaussian_vertex.shader", "./src/shaders/gaussian_fragment.shader");

    GLuint quad_vb  = create_gl_array_buffer(quad_vertices, 18 * sizeof(float));
    GLuint quad_tex = create_gl_array_buffer(quad_tex_coords, 12 * sizeof(float));

    GLuint gaus_shader = create_shader("./src/shaders/gaussian_vertex.shader", "./src/shaders/gaussian.shader");

    ///

    void *frame_buffer_data = (void*)malloc(WIDTH * HEIGHT * 3);

    model_matrix_edit = glm::scale(model_matrix_edit, glm::vec3(3,3,3));
    
    static bool first_pass = false;
    int pass_count = 0;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // first pass
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram); // light shader
        glBindTexture(GL_TEXTURE_2D, texture);
        
        mvp_matrix = projection_matrix * view_matrix * model_matrix_edit;
        load_uniform_mat4(shaderProgram, "mvp", &mvp_matrix[0][0]);
        load_uniform_mat4(shaderProgram, "model", &model_matrix_edit[0][0]);

        // draw sphere
        glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vb);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cube_color_vb);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, cube_texture_vb);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
		glDrawArrays(GL_TRIANGLES, 0, (vertices.size() * sizeof(glm::vec3)) / (sizeof(float)) / 3);
        /////
        glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

        // second pass (blur)
        bool horizontal = true, first_iteration = true;
        unsigned int amount = 10;
        glUseProgram(gaus_shader);
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]); 
            GLint hloc = glGetUniformLocation(gaus_shader, "horizontal");
            if(hloc == -1) fprintf(stderr, "'horizontal' uniform is not found.\n");
            glUniform1i(hloc, horizontal);
            glBindTexture(
                GL_TEXTURE_2D, first_iteration ? fbo_textures[1] : pingpongBuffer[!horizontal]
                );

            //render quad
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, quad_vb);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, quad_tex);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            //

            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        //

        
        // third pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(quad_shader);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo_textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]);

        GLint screenTexLoc = glGetUniformLocation(quad_shader, "screenTexture");
        GLint blurTexLoc = glGetUniformLocation(quad_shader, "blurTexture");
        glUniform1i(screenTexLoc, 0);
        glUniform1i(blurTexLoc, 1);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vb);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, quad_tex);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
        //

        // reset active texture
        glActiveTexture(GL_TEXTURE0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
// if(!first_pass){
//     first_pass = true;
//     glBindTexture(GL_TEXTURE_2D, fbo_texture);
//     glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer_data);
//     stbi_flip_vertically_on_write(1);
//     stbi_write_png("out2.png", WIDTH, HEIGHT, 3, (unsigned char*)frame_buffer_data, WIDTH * 3);
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

void handle_window_input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float yRotateAmout = 0.0f;
    float zRotateAmout = 0.0f;

    float rotateAmount = 10.0f;
    
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwTerminate();
		exit(0);
	}
    else if(key == GLFW_KEY_W && action == GLFW_PRESS){
        zRotateAmout += rotateAmount;
        glm::mat4 transform = glm::mat4(1.0f);
    }
    else if(key == GLFW_KEY_A && action == GLFW_PRESS){
        yRotateAmout -= rotateAmount;
    }
    else if(key == GLFW_KEY_D && action == GLFW_PRESS){
        yRotateAmout += rotateAmount;
    }

    model_matrix_edit = glm::rotate(model_matrix_edit,
                                    glm::radians(yRotateAmout),
                                    glm::vec3(0.0f, 0.0f, 1.0f));
    
    model_matrix_edit = glm::rotate(model_matrix_edit,
                                    glm::radians(zRotateAmout),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
