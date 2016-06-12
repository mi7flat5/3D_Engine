#ifndef MODEL_H
#define MODEL_H

#include"Mesh.h"
#include<vector>
#include"LoadUtility.h"
#include"Shaders.hpp"
#include"glm-0.9.2.7\glm\glm.hpp"


class Model {

	std::vector<Mesh> meshes;
	
	Shaders* ModelShader;
	GLuint SkyBox;
	GLuint SkyBoxReflect;
	GLuint ProjectionMatrixID;
	GLuint ViewMatrixID;
	GLint ModelMatrixID;
	GLint lightPosLoc;
	GLuint Actor;
	GLint viewPosLoc;
	GLint LC;
	GLuint ShaderModeLocation;
	GLuint ShaderMode;
	glm::vec3 lpos;
	glm::vec3 lightColor;
	GLuint DisplacementSamplerID;
	GLuint bDispMappingID;
	MaterialType ModelMaterial;
	

public:
	//determines which shader to load
	void SetMeshMode(int);
	static glm::mat4 View;
	glm::vec3 modelactor;
	Model(const std::string&);
	Model(const std::string & InPath, MaterialType);
	void SetViewMatrix(glm::mat4);
	~Model();
	void drawModel(glm::vec3&, glm::mat4&, glm::mat4&, glm::mat4 &)const;
	GLuint GetModelShaderID();
	void UpdateLightPosition(glm::vec4&);
	void SetActor(glm::vec3 actor);
};
#endif // !MODEL_H

