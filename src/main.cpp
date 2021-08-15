#include <iostream>
#include "glew.h"
#include "glfw3.h"
#include "shader.hpp"

#include "mat4x4.hpp"
#include "vec3.hpp"
#include "gtc/matrix_transform.hpp"


#include "cube.h"

#define WIDTH 800
#define HEIGHT 600

// static float g_vertex_buffer_data[] = {
//    -1.0f, -1.0f, 0.0f,
//    1.0f, -1.0f, 0.0f,
//    0.0f,  1.0f, 0.0f,
// };


int main(int argc, char *argv[])
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	
	// create cube buffer
	GLuint cube_vb;
	glGenBuffers(1, &cube_vb);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
				 g_vertex_buffer_data, GL_STATIC_DRAW);


    GLuint shaderProgram = LoadShaders("./src/shaders/vertex.shader",
									   "./src/shaders/frag.shader");

	// MATRICES STUFF
	glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f),
												   4.0f / 3.0f,
												   0.1f,
												   100.0f);
	
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(4,3,3),
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

		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data) / (sizeof(float)) / 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
