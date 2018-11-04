#ifndef G3DSimpleGeometry3D_H

#define G3DSimpleGeometry3D_H

#endif // !G3DSimpleGeometry3D_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"


class G3DMesh3D
{
public:
	std::vector<GLfloat> Vertices;
	std::vector<GLfloat> Normals;
	std::vector<GLfloat> UV;
	std::vector<GLushort> Indices;

	G3DMesh3D();
};

class G3DSimpleGeometry3D
{
public:

	G3DMesh3D mesh;
	std::vector<std::vector<float> > Transform;

	GLuint program;
	GLuint vao, vbo[3], ebo;
	GLint mvp_location, vpos_location, vuv_location, vN_location, veye_location, vlight_location;

	G3DSimpleGeometry3D();
	void AttachProgram(GLuint prog);
	void GenBuffers();
	void Render(glm::mat4 MVPmatrix, glm::uvec3 LIGHT, glm::uvec3 EYE);

};

G3DSimpleGeometry3D LoadG3DFile(const char *filePath);

std::vector<float> ReadNumbers(const char *filePath);