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
	
	GLuint MatrixID;
	GLint modelLoc;
	GLint lightPosLoc;
	GLint viewPosLoc;
	GLint LC;
	glm::vec3 lpos;
	glm::vec3 lightColor;

	MaterialType ModelMaterial;

public:
	//determines which shader to load
	
	Model(const std::string&);
	Model(const std::string & InPath, MaterialType);
	
	~Model();
	void drawModel(glm::vec3&, glm::mat4&, glm::mat4&)const;
	GLuint GetModelShaderID();
	void UpdateLightPosition(glm::vec3&);
};
#endif // !MODEL_H

