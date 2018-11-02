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


static const float dist = 50.0f;

static const GLfloat LIGHT[] = { 0.0f, -1.0f, -0.2f };

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
	GLuint vao, vbo[3];
	GLuint vbo_cube_vertices, vbo_cube_colors, vertex_shader, fragment_shader, program;
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

	/*if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW deu ruim" << std::endl;
		exit(EXIT_FAILURE);
	}*/

	G3DSimpleGeometry3D cube = LoadG3DFile("C:/CPP/OpenGL/OpenGlStudies/OpenGLFromScratch/OpenGLFromScratch/src/models/cube.g3d");

	program = LoadShader("C:/CPP/OpenGL/OpenGLStudies/OpenGLFromScratch/OpenGLFromScratch/src/shaders/phong.vertex",
		"C:/CPP/OpenGL/OpenGLStudies/OpenGLFromScratch/OpenGLFromScratch/src/shaders/phong.frag");

	LoadBMPCustom("C:/CPP/OpenGL/OpenGlStudies/OpenGLFromScratch/OpenGLFromScratch/src/models/cubeTexture.bmp");

	glUseProgram(program);

	vlight_location = glGetUniformLocation(program, "LIGHT");
	veye_location = glGetUniformLocation(program, "EYE");
	mvp_location = glGetUniformLocation(program, "MVP");

	vpos_location = glGetAttribLocation(program, "vPos");
	vN_location = glGetAttribLocation(program, "vN");
	vuv_location = glGetAttribLocation(program, "vUV");

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(3, vbo);

	//positions
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, cube.mesh.Vertices.size() * sizeof(GLfloat), &cube.mesh.Vertices[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vpos_location);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, cube.mesh.Normals.size() * sizeof(GLfloat), &cube.mesh.Normals[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vN_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vN_location);

	//uv
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	glBufferData(GL_ARRAY_BUFFER, cube.mesh.UV.size() * sizeof(GLfloat), &cube.mesh.UV[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vuv_location);



	/*glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(vpos_location);*/

	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	/*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);*/
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.mesh.Indices.size() * sizeof(GLushort), &cube.mesh.Indices[0], GL_DYNAMIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		// Keep running
		float ratio;
		int width, height;
		//mat4x4 m, v, p, mv, mvp;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		//glCullFace(GL_BACK);

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

		float angle = 1 / 1000.0 * 45;

		glm::mat4 Model, Projection, ModelRotation, MVPmatrix;
		glm::vec3 axis_y(0, 1, 0);

		Model = glm::mat4(1.0f);
		glm::rotate(glm::mat4(1.0f), glm::radians(angle / 1000 * 45), axis_y);
		glm::mat4 View = glm::lookAt(
			glm::vec3(EYE[0], EYE[1], EYE[2]), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		MVPmatrix = Projection * View * Model;// *ModelRotation;

		glUniform3f(vlight_location, LIGHT[0], LIGHT[1], LIGHT[2]);
		glUniform3f(veye_location, EYE[0], EYE[1], EYE[2]);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &MVPmatrix[0][0]);


		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//gluniformtexture2d

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		int size;  
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		//glDrawElements(GL_TRIANGLES, cube.mesh.Indices.size() * sizeof(GLfloat), GL_UNSIGNED_SHORT, (void*)0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
