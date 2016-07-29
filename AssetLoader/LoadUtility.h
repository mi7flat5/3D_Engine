#ifndef LOADUTILITY_H
#define LOADUTILITY_H

#include"Mesh.h"
#include<vector>
#include"include\assimp\Importer.hpp"
#include "include\assimp\scene.h"        
#include "include\assimp\postprocess.h"
#include"glm-0.9.2.7\glm\glm.hpp"
#include"glm-0.9.2.7\glm\gtc\matrix_transform.hpp"
#include"glm-0.9.2.7\glm\gtc\type_ptr.hpp"
#include<iostream>

class LoadUtility
{
	
	static GLint TextureFromFile(std::string texpath);
	static GLuint loadCubemap();
	static std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, MeshType, std::vector<Texture>&, bool );

public:
	
	LoadUtility();
	~LoadUtility();
	static void loadHeightMap(std::string,int&,int&, std::vector< std::vector<GLfloat> >&);
	static unsigned char * loadHeightMap(std::string Path, int & Width, int & Height);
	static void LoadCollider(const std::string &path,std::vector<glm::vec3>& InVerts, std::vector<GLuint>& InIndices,std::vector<glm::vec3> &InRenderVerts);
	static void loadModel(std::vector<Mesh> &InMeshVec,const std::string&,MeshType);
};

#endif // !LOADUTILITY_H