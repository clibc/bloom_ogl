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

#define WIDTH 800
#define HEIGHT 600

void handle_window_input(GLFWwindow* window, int key, int scancode, int action, int mods);

static glm::mat4 model_matrix_edit = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, -2.0f));

int main(int argc, char *argv[])
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texturecoords;
	std::vector<glm::vec3> normals;
	bool load_result = load_obj_file_basic("build/texturedCube.obj",
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
	
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(1,4,6),
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
    unsigned char *texture_data = create_texture_rgb("build/texture.jpg", &width, &height, &channels);

    while (!glfwWindowShouldClose(window))
    {
        mvp_matrix = projection_matrix * view_matrix * model_matrix_edit;
        load_uniform_mat4(shaderProgram, "mvp", &mvp_matrix[0][0]);
        load_uniform_mat4(shaderProgram, "model", &model_matrix_edit[0][0]);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw cube
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

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

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
