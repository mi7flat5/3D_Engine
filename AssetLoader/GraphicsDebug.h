#pragma once
#include<vector>
#include<string>

#include"glm-0.9.2.7\glm\glm.hpp"
#include"glm-0.9.2.7\glm\gtc\matrix_transform.hpp"
#include"glm-0.9.2.7\glm\gtc\type_ptr.hpp"
//#include<GL/glew.h>
#include"Shaders.hpp"
enum class dbgType {
	POINT,
	LINE,
	MESH
};
class GraphicsDebug
{
	std::vector<glm::vec3> dbgPoints;
	GLuint VAO, VBO, EBO,
		ProjectionMatrixID, 
		ViewMatrixID, 
		ModelMatrixID;
	dbgType PrimType;
	Shaders* ModelShader;

public:
	GraphicsDebug();
	void Draw(glm::vec3&);
	GraphicsDebug(glm::vec3 &Point);
	GraphicsDebug(std::vector<glm::vec3> &Points, dbgType Type);
	
	void Draw(glm::mat4 &ModelMatrix);
	~GraphicsDebug();
};

