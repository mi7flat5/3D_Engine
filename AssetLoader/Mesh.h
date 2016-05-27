#ifndef MESH_H
#define MESH_H
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 diffuse;
	glm::vec2 texCoord;
};

struct Texture {
	GLuint id;
	std::string type;
	std::string texPath;
};
class Mesh
{
	std::vector<Vertex>verts;
	std::vector<GLuint>indices;
	std::vector<Texture>textures;
	GLuint VAO, VBO, EBO,ModelLoc;

public:

	Mesh(const std::vector<Vertex> &inVerts,const std::vector<GLuint> &inIndice,const std::vector<Texture> &inTextures);
	~Mesh();
	void setupMesh();
	void drawMesh(GLuint) const;
	void drawMesh() const;
};
#endif // !MESH_H
