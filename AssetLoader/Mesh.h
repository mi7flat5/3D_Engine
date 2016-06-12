#ifndef MESH_H
#define MESH_H
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vector>
//Shader selector for Model/Mesh
//TEXTURE_2D_HIDDEN_REFLECT is for reflection of undisplayed skybox
// NO_TEXTURE is for model without texture coorinates, extracts diffuse from mesh
//TEXTURE_3D is for Skybox cubemaps
enum class MaterialType { 
		NO_TEXTURE, 
		TEXTURE_2D, 
		TEXTURE_2D_DISPLACEMENT,
		TEXTURE_2D_REFRACT, 
		TEXTURE_2D_REFLECT,
		TEXTURE_2D_HIDDEN_REFLECT, 
		TEXTURE_3D_REFLECTOR, 
		TEXTURE_3D
};
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 diffuse;
	glm::vec2 texCoord;
	glm::vec3 Tangent;
	glm::vec3 BitTangent;
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
	GLint ShaderMode;
	GLuint VAO, VBO, EBO,ModelLoc, ShaderModeLocation;
	void setupMesh(MaterialType);
	void SetupCubemap();
public:
	void SetMode(int);
	Mesh(const std::vector<Vertex> &inVerts,const std::vector<GLuint> &inIndice,const std::vector<Texture> &inTextures,MaterialType);
	~Mesh();
	
	void drawMesh(GLuint,MaterialType) const;
	void drawMesh() const;
	void SetTextureMode();
};
#endif // !MESH_H
