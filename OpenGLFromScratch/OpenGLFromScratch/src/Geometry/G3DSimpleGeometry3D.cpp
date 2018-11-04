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

void G3DSimpleGeometry3D::AttachProgram(GLuint prog) {
	program = prog;
	GenBuffers();
}

void G3DSimpleGeometry3D::GenBuffers() {
	
	vlight_location = glGetUniformLocation(program, "LIGHT");
	veye_location = glGetUniformLocation(program, "EYE");
	mvp_location = glGetUniformLocation(program, "MVP");

	vpos_location = glGetAttribLocation(program, "vPos");
	vN_location = glGetAttribLocation(program, "vN");
	vuv_location = glGetAttribLocation(program, "vUV");

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(3, vbo);

	glGenBuffers(1, &ebo);

	//positions
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(GLfloat), &mesh.Vertices[0], GL_DYNAMIC_DRAW);


	//normals
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, mesh.Normals.size() * sizeof(GLfloat), &mesh.Normals[0], GL_DYNAMIC_DRAW);

	//uv
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	glBufferData(GL_ARRAY_BUFFER, mesh.UV.size() * sizeof(GLfloat), &mesh.UV[0], GL_DYNAMIC_DRAW);
	
	//index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(GLushort), &mesh.Indices[0], GL_DYNAMIC_DRAW);
}

void G3DSimpleGeometry3D::Render(glm::mat4 MVPmatrix, glm::uvec3 LIGHT, glm::uvec3 EYE) {
	
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vpos_location);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(vN_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vN_location);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vuv_location);

	glUniform3f(vlight_location, LIGHT[0], LIGHT[1], LIGHT[2]);
	glUniform3f(veye_location, EYE[0], EYE[1], EYE[2]);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &MVPmatrix[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}

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
					//std::vector<GLfloat> uv({ 1, 1, (GLfloat)0.0 });
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
