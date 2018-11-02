#ifndef G3DSimpleGeometry3D_H

#define G3DSimpleGeometry3D_H

#endif // !G3DSimpleGeometry3D_H

#include <vector>

#include <GLFW/glfw3.h>


//class G3DVector3D
//{
//public:
//	GLfloat X, Y, Z;
//
//	G3DVector3D();
//
//	G3DVector3D(GLfloat x, GLfloat y, GLfloat z);
//};

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

	G3DSimpleGeometry3D();

};

G3DSimpleGeometry3D LoadG3DFile(const char *filePath);

std::vector<float> ReadNumbers(const char *filePath);