#include "LoadUtility.h"
#include<SOIL.h>

LoadUtility::LoadUtility() { bNormalsLoaded = false; }
LoadUtility::~LoadUtility(){}

void LoadUtility::loadModel( std::vector<Mesh> &InMeshVec,const std::string &path,MaterialType TextureType)
{
	std::vector<Vertex>verts;
	std::vector<GLuint>indices;
	std::vector<Texture> textures;
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_FlipUVs
	);
	
	if (!scene)
	{
		std::cout << "\nFailed to load! ";
		return;
	}

	
	for (GLuint i = 0;i <scene->mNumMeshes;++i)
	{
		
		aiMesh* tmpMesh = scene->mMeshes[(scene->mNumMeshes - 1) - i];
		aiFace  tmpFace;
		for (int j = 0;j < tmpMesh->mNumVertices;++j)
		{
			Vertex tmpVert;
			glm::vec3 tmpPos;
			tmpPos.x = tmpMesh->mVertices[j].x;
			tmpPos.y = tmpMesh->mVertices[j].y;
			tmpPos.z = tmpMesh->mVertices[j].z;
			if (TextureType == MaterialType::TEXTURE_2D_DISPLACEMENT)
				std::cout <<"\n"<< tmpPos.x<<"," << tmpPos.y << ", " << tmpPos.z;;
			
			glm::vec3 tmpNorm;
			if (tmpMesh->HasNormals())
			{
				tmpNorm.x = tmpMesh->mNormals[j].x;
				tmpNorm.y = tmpMesh->mNormals[j].y;
				tmpNorm.z = tmpMesh->mNormals[j].z;
			}
			
			glm::vec3 tmpTang, tmpBitTang;
			if (tmpMesh->HasTangentsAndBitangents())
			{
				tmpTang.x = tmpMesh->mTangents[j].x;
				tmpTang.y = tmpMesh->mTangents[j].y;
				tmpTang.z = tmpMesh->mTangents[j].z;
				tmpBitTang.x = tmpMesh->mBitangents[j].x;
				tmpBitTang.y = tmpMesh->mBitangents[j].y;
				tmpBitTang.z = tmpMesh->mBitangents[j].z;
			}
			

			glm::vec3 tmpCol;
			aiColor3D aiCol;
			aiMaterial *tmpMat = scene->mMaterials[tmpMesh->mMaterialIndex];
			tmpMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiCol);
			tmpCol.r = aiCol.r;
			tmpCol.g = aiCol.g;
			tmpCol.b = aiCol.b;

			tmpVert.Position = tmpPos;
			tmpVert.Normal = tmpNorm;
			tmpVert.diffuse = tmpCol;
			tmpVert.Tangent = tmpTang;
			tmpVert.BitTangent = tmpBitTang;

			if (tmpMesh->mTextureCoords[0] && TextureType != MaterialType::TEXTURE_3D)
			{
				tmpVert.texCoord.x = tmpMesh->mTextureCoords[0][j].x;
				tmpVert.texCoord.y = tmpMesh->mTextureCoords[0][j].y;
			}
			else 
			{
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
			aiMaterial* material = scene->mMaterials[tmpMesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN
			// Displacement map, heightmap, depthmap etc. 

			// 1. Diffuse maps
			std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "material.texture_diffuse",TextureType);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "material.texture_specular", TextureType);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. Normal maps ATTENTION! aiTextureType_NORMAL may work with fbx, aiTexturType_HEIGHT may be an option
			std::vector<Texture> normalMaps = this->loadMaterialTextures(material, aiTextureType_NORMALS, "material.texture_normal", TextureType);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			bNormalsLoaded = true;
			// 4. Displacement maps
			//hack because blender and/or assimp does not export/import displacement maps in fbx 
			
			if (TextureType == MaterialType::TEXTURE_2D_DISPLACEMENT)
			{
				std::vector<Texture> displacementMaps = this->loadMaterialTextures(material, aiTextureType_NORMALS, "material.texture_displacement", TextureType);
				textures.insert(textures.end(), displacementMaps.begin(), displacementMaps.end());
			}
			std::cout << "\n" << material->mProperties[i]->mType;
		}
		if (TextureType == MaterialType::TEXTURE_3D)//TODO Add reflecive environment cubmap option here
		{
			textures.clear();
			textures.push_back(Texture());
			textures[0].type = "CubeMap";
			textures[0].id = loadCubemap();
			
		}
		for (int i = 0; i<textures.size();i++)
			std::cout << "\n" << textures[i].type;
		InMeshVec.push_back(Mesh(verts, indices, textures,TextureType));
		verts.clear();
		indices.clear();
		textures.clear();
	}
	

}

std::vector<Texture> LoadUtility::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, MaterialType TextureType)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		
		mat->GetTexture(type, i, &str);
		std::string tmpTexPath = str.C_Str();
		std::cout << "\n material type file: " << tmpTexPath;
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		//Hack for displacement maps continues, normalmap must have "_n" at end of name, and displacement map must be the same name with
		// "_d" at the end of filename
		if (TextureType == MaterialType::TEXTURE_2D_DISPLACEMENT && bNormalsLoaded)
		{
			std::string key("_n");
			std::size_t found = tmpTexPath.rfind(key);
			if (found != std::string::npos)
				tmpTexPath.replace(found, key.length(), "_d");
			typeName = "material.texture_displacement";

		}
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
GLint LoadUtility::TextureFromFile(std::string texpath) //TODO Add exception for loading wrong resolutions 
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	std::cout << "\nTexture Loaded: " + FileName;
	SOIL_free_image_data(image);
	
	return textureID;
}
GLuint LoadUtility::loadCubemap()
{
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	std::vector<const GLchar*> faces;
	faces.push_back("media/right.jpg");
	faces.push_back("media/left.jpg");
	faces.push_back("media/top.jpg");
	faces.push_back("media/bottom.jpg");
	faces.push_back("media/back.jpg");
	faces.push_back("media/front.jpg");
	
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
		
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	return textureID;
}