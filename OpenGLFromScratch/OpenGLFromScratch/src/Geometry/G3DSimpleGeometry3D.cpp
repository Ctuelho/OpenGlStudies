#include "G3DSimpleGeometry3D.h"

#include <fstream>
#include <iostream>
#include <string>

G3DSimpleGeometry3D::G3DSimpleGeometry3D(void) 
{
	GLfloat identityTransform[] = {
						1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0 };

	this->Transform.insert(
		this->Transform.end(), &identityTransform[0],
		&identityTransform[sizeof(identityTransform) / sizeof(identityTransform[0])]);
};

//G3DVector3D::G3DVector3D() {
//	this->X = this->Y = this->Z = GLfloat(0.0);
//};
//
//G3DVector3D::G3DVector3D(GLfloat x, GLfloat y, GLfloat z)
//{
//	this->X = x;
//	this->Y = y;
//	this->Z = z;
//};

G3DMesh3D::G3DMesh3D(void) {};

G3DSimpleGeometry3D LoadG3DFile(const char *filePath) {

	G3DSimpleGeometry3D result;

	std::ifstream inFile(filePath, std::ios::in);

	if (inFile.is_open())
	{
		std::string line = "";
		while (!inFile.eof()) {
			std::getline(inFile, line);

			if (strlen(&line[0]) == 0)
			{
				continue;
			}
			else if (line[0] == '#')
			{
				continue;
			}
			std::vector<GLfloat> numbers(ReadNumbers(&line[0]));

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{
					std::vector<GLfloat> uv({ numbers[0], numbers[1], (GLfloat)0.0 });
					result.mesh.UV.insert(result.mesh.UV.end(), uv.begin(), uv.end());
				}
				else if (line[1] == 'n')
				{
					std::vector<GLfloat> normal({ numbers[0], numbers[1], numbers[2] });
					result.mesh.Normals.insert(result.mesh.Normals.end(), normal.begin(), normal.end());
				}
				else
				{
					std::vector<GLfloat> vertex({ numbers[0], numbers[1], numbers[2] });
					result.mesh.Vertices.insert(result.mesh.Vertices.end(), vertex.begin(), vertex.end());
				}
			}
			else if (line[0] == 'f')
			{
				std::vector<GLuint> face({ (GLuint)numbers[0], (GLuint)numbers[1], (GLuint)numbers[2] });
				result.mesh.Indices.insert(result.mesh.Indices.end(), face.begin(), face.end());
			}

		}
	}
	else
	{
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
	}

	inFile.close();
	return result;
};

std::vector<GLfloat> ReadNumbers(const char *stringToConvert)
{
	std::vector<GLfloat> numbers;
	int indexOnString;
	int nextSpaceIndex;

	indexOnString = 0;

	while (strlen(stringToConvert) != 0 && indexOnString < strlen(stringToConvert))
	{
		if (stringToConvert[indexOnString] == ' ')
		{
			nextSpaceIndex = ++indexOnString;
			while (stringToConvert[nextSpaceIndex] != ' ')
			{
				nextSpaceIndex++;
			}
			std::string stringToBeConverted = std::string(stringToConvert, indexOnString, nextSpaceIndex - indexOnString);

			numbers.push_back(std::stof(stringToBeConverted));

			indexOnString = nextSpaceIndex;
		}
		else
		{
			indexOnString++;
		}
	}

	return numbers;
};
