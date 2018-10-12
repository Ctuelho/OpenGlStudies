#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <stdio.h>

#include "ShaderCompiler.hpp"

#include "linmath.h"

#include "glm.hpp"
#include "matrix_transform.hpp"


static const float dist = 50.0f;

static const GLfloat cube_vertices[] = {
	// front
	 0.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,
	 1.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,
	// back
	 0.0,  0.0, -1.0,
	 1.0,  0.0, -1.0,
	 1.0,  1.0, -1.0,
	 0.0,  1.0, -1.0,
};

static const GLfloat cube_colors[] = {
	// Cores de frente
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// Cores do Fundo
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
};

static unsigned int indices[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// top
	3, 2, 6,
	6, 7, 3,
	// back
	7, 6, 5,
	5, 4, 7,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// left
	4, 0, 3,
	3, 7, 4,
	// right
	1, 5, 6,
	6, 2, 1,
};

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	
	GLFWwindow* window;
	GLuint vao, vbo[2];
	GLuint vbo_cube_vertices, vbo_cube_colors, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	if (!glfwInit())
	{
		std::cout << "GLFW deu ruim" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

	if (!window)
	{
		std::cout << "WINDOW deu ruim" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	/*if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW deu ruim" << std::endl;
		exit(EXIT_FAILURE);
	}*/

	program = LoadShader("C:/CPP/OpenGL/OpenGLFromScratch/OpenGLFromScratch/src/shaders/phong.vertex",
		"C:/CPP/OpenGL/OpenGLFromScratch/OpenGLFromScratch/src/shaders/phong.frag");

	glUseProgram(program);


	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vpos_location);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vcol_location);

	/*glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(vpos_location);*/

	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);


	while (!glfwWindowShouldClose(window))
	{
		// Keep running
		float ratio;
		int width, height;
		//mat4x4 m, v, p, mv, mvp;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glCullFace(GL_NONE);

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		//glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//mat4x4_rotate_Z(m, m, (float)glfwGetTime());
		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		
		/*mat4x4_identity(m);
		mat4x4_look_at(v, new vec3{ 0.0f, 2.0f, 4.0f }, new vec3{ 0.0f, 0.0f, -4.0f }, new vec3{ 0.0f, 1.0f, 0.0f });
		mat4x4_perspective(p, 45.0f, ratio, 0, 100);
		mat4x4_mul(mv, m, v);
		mat4x4_mul(mvp, mv, p);*/

		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		glm::mat4 MVPmatrix = Projection * View * Model;

		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &MVPmatrix[0][0]);


		//glDrawArrays(GL_TRIANGLES, 0, 3);


		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_SHORT, (void*)0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
