#pragma once
#include"Mesh.h"

#include<vector>
#include"include\assimp\Importer.hpp"
#include "include\assimp\scene.h"        
#include "include\assimp\postprocess.h"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<iostream>
class LoadUtility
{
	std::vector<Mesh> meshes;
	std::vector<Texture> texRecord;
	GLint TextureFromFile(std::string texpath);
	GLuint loadCubemap();
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, MaterialType );
	bool bNormalsLoaded;
public:
	
	LoadUtility();
	~LoadUtility();
	
	void loadModel(std::vector<Mesh> &InMeshVec,const std::string&,MaterialType);
};

