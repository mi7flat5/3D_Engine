#include"Model.h"


Model::Model(const std::string &InPath) 
{
	/*LoadUtility *Loader = new LoadUtility;
	Loader->loadModel(meshes, InPath);
	delete Loader;*/ //TODO fix later
}
Model::Model(const std::string &InPath, MaterialType shader)
{
	ModelMaterial = shader;
	LoadUtility *Loader = new LoadUtility;
	Loader->loadModel(meshes, InPath,shader);
	delete Loader;
	
	if (shader == MaterialType::TEXTURE_2D)
		ModelShader = new Shaders("shaders/texVert.glsl", "shaders/texFrag.glsl");
	if (shader == MaterialType::NO_TEXTURE)
		ModelShader = new Shaders("shaders/vert.glsl", "shaders/frag.glsl");
	if (shader == MaterialType::TEXTURE_3D)
		ModelShader = new Shaders("shaders/3Dvert.glsl", "shaders/3Dfrag.glsl");
	
	lpos = glm::vec3(0, 20, 20);
	lightColor = glm::vec3(1, 1, 1);
	MatrixID = glGetUniformLocation(ModelShader->getProgram(), "MVP");
	modelLoc = glGetUniformLocation(ModelShader->getProgram(), "Model");
	lightPosLoc = glGetUniformLocation(ModelShader->getProgram(), "lightPos");
	viewPosLoc = glGetUniformLocation(ModelShader->getProgram(), "viewPos");
	LC = glGetUniformLocation(ModelShader->getProgram(), "lightColor");
}
Model::~Model()
{
	glDeleteProgram(ModelShader->getProgram());
	delete ModelShader;
}

void Model::drawModel(glm::vec3 &campos,glm::mat4 &Model,glm::mat4 & MVP)const 
{
	if (ModelMaterial == MaterialType::TEXTURE_3D)
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		ModelShader->Use();
		glUniform1i(glGetUniformLocation(ModelShader->getProgram(), "skybox"), 0);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &Model[0][0]);
		for (int i = 0;i < meshes.size();i++)
			meshes[i].drawMesh(ModelShader->getProgram(), ModelMaterial);
		
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}
	else 
	{
		ModelShader->Use();
		glUniform3fv(LC, 1, &lightColor[0]);
		glUniform3f(lightPosLoc, lpos.x, lpos.y, lpos.z);
		glUniform3f(viewPosLoc, campos.x, campos.y, campos.z);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &Model[0][0]);
		for (int i = 0;i < meshes.size();++i)
			meshes[i].drawMesh(ModelShader->getProgram(), ModelMaterial);
	}
	
	
	

	
}
GLuint Model::GetModelShaderID() 
{
	return ModelShader->getProgram();
}
void Model::UpdateLightPosition(glm::vec3 &LightPos)

{
	lpos = LightPos;
}








