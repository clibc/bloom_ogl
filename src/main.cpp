#include <iostream>
#include "glew.h"
#include "glfw3.h"
#include "shader.hpp"

#include "mat4x4.hpp"
#include "vec3.hpp"
#include "gtc/matrix_transform.hpp"

#include "cube.h"
#include "objloader.hpp"

#define WIDTH 800
#define HEIGHT 600

void handle_window_input(GLFWwindow* window, int key, int scancode, int action, int mods);
	
int main(int argc, char *argv[])
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texturecoords;
	std::vector<glm::vec3> normals;
	bool load_result = load_obj_file_basic("build/test.obj",
						vertices,
						texturecoords,
						normals);

	if(!load_result) { exit(-1); }

	GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "loser engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, handle_window_input);
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	// create cube buffers

	// vertex buffer
	GLuint cube_vb;
	glGenBuffers(1, &cube_vb);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vb);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
				 &vertices[0], GL_STATIC_DRAW);

	GLuint cube_color_vb;
	glGenBuffers(1, &cube_color_vb);
	glBindBuffer(GL_ARRAY_BUFFER, cube_color_vb);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
				 &normals[0], GL_STATIC_DRAW);

	
	///

    GLuint shaderProgram = LoadShaders("./src/shaders/vertex.shader",
									   "./src/shaders/frag.shader");

	// MATRICES STUFF
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f),
												   4.0f / 3.0f,
												   0.1f,
												   100.0f);
	
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(2,4,6),
								 glm::vec3(0,0,0),
								 glm::vec3(0,1,0));
	glm::mat4 model_matrix = glm::mat4(1.0f);

	glm::mat4 mvp_matrix = projection_matrix * view_matrix * model_matrix;
	
	//

	glUseProgram(shaderProgram);

	GLuint matrixID = glGetUniformLocation(shaderProgram, "mvp");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp_matrix[0][0]);

	
    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw cube
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vb);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cube_color_vb);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, (vertices.size() * sizeof(glm::vec3)) / (sizeof(float)) / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void handle_window_input(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwTerminate();
		exit(0);
	}
}
