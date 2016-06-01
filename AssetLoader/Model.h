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
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLint ModelMatrixID;
	GLint lightPosLoc;
	GLuint Actor;
	GLint viewPosLoc;
	GLint LC;
	glm::vec3 lpos;
	glm::vec3 lightColor;
	glm::mat4 View;
	MaterialType ModelMaterial;

public:
	//determines which shader to load
	glm::vec3 modelactor;
	Model(const std::string&);
	Model(const std::string & InPath, MaterialType);
	void SetViewMatrix(glm::mat4);
	~Model();
	void drawModel(glm::vec3&, glm::mat4&, glm::mat4&)const;
	GLuint GetModelShaderID();
	void UpdateLightPosition(glm::vec3&);
	void SetActor(glm::vec3 actor);
};
#endif // !MODEL_H

