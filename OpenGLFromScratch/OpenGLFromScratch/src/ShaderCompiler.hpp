#ifndef GLSHADER_H
#define GLSHADER_H

#include <glad/glad.h>

GLuint LoadShader(const char *vertex_path, const char *fragment_path);

#endif

//
//static unsigned int CompileShader(unsigned int type, const std::string& source);
//
//static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);