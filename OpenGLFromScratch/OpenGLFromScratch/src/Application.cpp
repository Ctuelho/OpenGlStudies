#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <stdio.h>

#include "ShaderCompiler.hpp"

#include "linmath.h"

#include "glm.hpp"
#include "matrix_transform.hpp"

#include "Geometry/G3DSimpleGeometry3D.h"

#include "BMPLoader.h"



static const GLfloat LIGHT[] = { 4.0f, 3.0f, 3.0f };

static const GLfloat EYE[]= { 4.0f, 3.0f, 3.0f };

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
	GLuint vertex_shader, fragment_shader, program;
	GLuint simpleWater_vertex_shader, simpleWater_fragment_shader, simpleWater_program;
	GLint mvp_location, vpos_location, vuv_location, vN_location, veye_location, vlight_location;
	// Create one OpenGL texture

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

	program = LoadShader("C:/CPP/OpenGL/OpenGLStudies/OpenGLFromScratch/OpenGLFromScratch/src/shaders/phong.vertex",
		"C:/CPP/OpenGL/OpenGLStudies/OpenGLFromScratch/OpenGLFromScratch/src/shaders/phong.frag");

	simpleWater_program = LoadShader("C:/CPP/OpenGL/OpenGLStudies/OpenGLFromScratch/OpenGLFromScratch/src/shaders/simpleWater.vertex",
		"C:/CPP/OpenGL/OpenGLStudies/OpenGLFromScratch/OpenGLFromScratch/src/shaders/simpleWater.frag");

	G3DSimpleGeometry3D cube = LoadG3DFile("C:/CPP/OpenGL/OpenGlStudies/OpenGLFromScratch/OpenGLFromScratch/src/models/cube.g3d");
	cube.AttachProgram(program);

	G3DSimpleGeometry3D plane = LoadG3DFile("C:/CPP/OpenGL/OpenGlStudies/OpenGLFromScratch/OpenGLFromScratch/src/models/plane.g3d");
	plane.AttachProgram(simpleWater_program);

	while (!glfwWindowShouldClose(window))
	{
		// Keep running
		float ratio;
		int width, height;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		//glCullFace(GL_BACK);

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 Model, View, Projection, MVPmatrix;

		Model = glm::mat4(1.0f);
		View = glm::lookAt(
			glm::vec3(EYE[0], EYE[1], EYE[2]), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		MVPmatrix = Projection * View * Model;

		LoadBMPCustom("C:/CPP/OpenGL/OpenGlStudies/OpenGLFromScratch/OpenGLFromScratch/src/models/planeTexture.bmp");
		plane.Render(MVPmatrix, glm::vec3(LIGHT[0], LIGHT[1], LIGHT[2]), glm::vec3(EYE[0], EYE[1], EYE[2]));

		LoadBMPCustom("C:/CPP/OpenGL/OpenGlStudies/OpenGLFromScratch/OpenGLFromScratch/src/models/cubeTexture.bmp");
		cube.Render(MVPmatrix, glm::vec3(LIGHT[0], LIGHT[1], LIGHT[2]), glm::vec3(EYE[0], EYE[1], EYE[2]));

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
