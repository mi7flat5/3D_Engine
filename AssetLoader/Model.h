#ifndef MODEL_H
#define MODEL_H

#include"Mesh.h"
#include<vector>
#include"LoadUtility.h"
#include"Shaders.hpp"
#include"glm-0.9.2.7\glm\glm.hpp"
#include"SHCamera.h"

class Model {
protected:
	GLuint SkyBox;//Move to Cubmap
	GLuint SkyBoxReflect;//also
	
	//Needed in all
	std::vector<Mesh> meshes;
	GLuint ProjectionMatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	MeshType ModelMaterial;
	Shaders* ModelShader;//Has to be set in base class constructor

	GLint lightPosLoc;

	GLint viewPosLoc;
	GLint LC;
	GLuint ShaderModeLocation;//remove
	GLuint ShaderMode;//remove
	GLuint NormMappingID, DispLevelID, SpecularShininessID; // move to 
	bool IsNormMapping;
	glm::vec3 lpos;
	glm::vec3 lightColor;
	GLuint DisplacementSamplerID;
	GLuint bDispMappingID;
	glm::vec3 Position;
	GLfloat DispLevel, Shininess;
	
public:
	//determines which shader to load,loads models and meshes for all subclasses. 
	Model(const std::string & InPath, MeshType);
	~Model();
	
	virtual void Draw(glm::vec3 & campos, glm::mat4 & Model);

	virtual void SetPosition(glm::vec3 InPos);

	virtual void SetScale(glm::mat4 InScale);

	void SetShaders(Shaders *InShader);
		
	
};
#endif // !MODEL_H

