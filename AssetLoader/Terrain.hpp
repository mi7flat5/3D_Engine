#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include"Model.h"

class Terrain : protected Model {

	GLint Width, Height;
	GLuint Scale, LastValidHeight;
	glm::mat4 TerrainModel;
	GLfloat MaxX, MaxZ, MinX, MinZ;
	unsigned char * HeightMapImage;
	
	void SetMinMaxBoundry();

public:
	Terrain(const std::string &InPath, MeshType shader);
	~Terrain();
	GLuint GetHeight(glm::vec3);
	GLfloat GetMaxX()const;
	GLfloat GetMaxZ()const;
	GLfloat GetMinX()const;
	GLfloat GetMinZ()const;
	
	void Draw() const;
	void SetScale(GLuint InScale);
};
#endif