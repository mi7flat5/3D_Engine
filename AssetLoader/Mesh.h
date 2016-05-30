#ifndef MESH_H
#define MESH_H
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vector>
enum class MaterialType { NO_TEXTURE, TEXTURE_2D, TEXTURE_3D };
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
	void setupMesh(MaterialType);
	void SetupCubemap();
public:

	Mesh(const std::vector<Vertex> &inVerts,const std::vector<GLuint> &inIndice,const std::vector<Texture> &inTextures,MaterialType);
	~Mesh();
	
	void drawMesh(GLuint,MaterialType) const;
	void drawMesh() const;
};
#endif // !MESH_H
