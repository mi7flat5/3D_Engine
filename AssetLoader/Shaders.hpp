#ifndef SHADERS_HPP
#define SHADERS_HPP



#include <GL/glew.h>

class Shaders
{
	
	GLuint Program;

public:
	
	Shaders(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
	GLuint getProgram();
};

#endif