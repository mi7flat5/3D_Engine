#include "LoadUtility.h"
#include<SOIL.h>

LoadUtility::LoadUtility(){}
LoadUtility::~LoadUtility(){}

void LoadUtility::loadModel( std::vector<Mesh> &InMeshVec,const std::string &path,MaterialType TextureType)
{
	std::vector<Vertex>verts;
	std::vector<GLuint>indices;
	std::vector<Texture> textures;
	Assimp::Importer importer;

	const aiScene *scene2 = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_FlipUVs
	);

	if (!scene2)
	{
		std::cout << "\nFailed to load! ";
		return;
	}


	for (unsigned int i = 0;i <scene2->mNumMeshes;++i)
	{
		aiMesh* tmpMesh = scene2->mMeshes[(scene2->mNumMeshes - 1) - i];
		aiFace  tmpFace;
		for (int j = 0;j < tmpMesh->mNumVertices;++j)
		{
			Vertex tmpVert;
			glm::vec3 tmpPos;
			tmpPos.x = tmpMesh->mVertices[j].x;
			tmpPos.y = tmpMesh->mVertices[j].y;
			tmpPos.z = tmpMesh->mVertices[j].z;
			glm::vec3 tmpNorm;
			if (tmpMesh->HasNormals())
			{
				tmpNorm.x = tmpMesh->mNormals[j].x;
				tmpNorm.y = tmpMesh->mNormals[j].y;
				tmpNorm.z = tmpMesh->mNormals[j].z;
			}
			glm::vec3 tmpCol;
			aiColor3D aiCol;
			aiMaterial *tmpMat = scene2->mMaterials[tmpMesh->mMaterialIndex];
			tmpMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiCol);
			tmpCol.r = aiCol.r;
			tmpCol.g = aiCol.g;
			tmpCol.b = aiCol.b;
			tmpVert.Position = tmpPos;
			tmpVert.Normal = tmpNorm;
			tmpVert.diffuse = tmpCol;
			if (tmpMesh->mTextureCoords[0] && TextureType != MaterialType::TEXTURE_3D)
			{
				tmpVert.texCoord.x = tmpMesh->mTextureCoords[0][j].x;
				tmpVert.texCoord.y = tmpMesh->mTextureCoords[0][j].y;
			}
			else {
				tmpVert.texCoord.x = 0.0f;
				tmpVert.texCoord.y = 0.0f;
			}

			verts.push_back(tmpVert);

		}
		for (int j = 0; j < tmpMesh->mNumFaces;++j)
		{
			tmpFace = tmpMesh->mFaces[j];
			for (int k = 0;k < tmpFace.mNumIndices;++k)
			{
				indices.push_back(tmpFace.mIndices[k]);
			}
		}
		if (tmpMesh->mMaterialIndex >= 0&&TextureType!=MaterialType::TEXTURE_3D)
		{
			aiMaterial* material = scene2->mMaterials[tmpMesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "material.texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "material.texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		if (TextureType == MaterialType::TEXTURE_3D)
		{
			textures.clear();
			textures.push_back(Texture());
			textures[0].id = loadCubemap();
			std::cout << "TexID from Loader main " << textures[0].id << "'\n";
		}
		InMeshVec.push_back(Mesh(verts, indices, textures,TextureType));
		verts.clear();
		indices.clear();
		textures.clear();
	}
	

}

std::vector<Texture> LoadUtility::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string tmpTexPath = str.C_Str();
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < texRecord.size(); j++)
		{
			if (texRecord[j].texPath == tmpTexPath)
			{
				textures.push_back(texRecord[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(tmpTexPath);
			texture.type = typeName;
			texture.texPath = tmpTexPath;
			textures.push_back(texture);
			this->texRecord.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}
GLint LoadUtility::TextureFromFile(std::string texpath)
{
	
	std::size_t found = texpath.find_last_of("/\\");
	std::string FileName = texpath.substr(found + 1);
	
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	FileName = "media/" + FileName;
	unsigned char* image = SOIL_load_image(FileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
																								// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	std::cout << "\nTexture Loaded: " + FileName;
	SOIL_free_image_data(image);
	
	return textureID;
}
GLuint LoadUtility::loadCubemap()
{
	std::vector<const GLchar*> faces;
	faces.push_back("media/right.jpg");
	faces.push_back("media/left.jpg");
	faces.push_back("media/top.jpg");
	faces.push_back("media/bottom.jpg");
	faces.push_back("media/back.jpg");
	faces.push_back("media/front.jpg");
	
	
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	//glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
		SOIL_free_image_data(image);
		std::cout << "\nTexture Loaded: " + std::string(faces[i])<<"\n";
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	std::cout << "TexID from Loader " << textureID << "'\n";
	return textureID;
}