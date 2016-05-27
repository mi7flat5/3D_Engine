#include"Model.h"


Model::Model(const std::string &InPath) 
{
	LoadUtility *Loader = new LoadUtility;
	Loader->loadModel(meshes, InPath);
	delete Loader;
}
Model::Model(const std::string &InPath, const std::string &VertexShader, const std::string &FragmnetShader)
{
	LoadUtility *Loader = new LoadUtility;
	Loader->loadModel(meshes, InPath);
	delete Loader;
	ModelShader = new Shaders(VertexShader.c_str(), FragmnetShader.c_str());
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
	glUniform3fv(LC, 1, &lightColor[0]);
	glUniform3f(lightPosLoc, lpos.x, lpos.y, lpos.z);
	glUniform3f(viewPosLoc, campos.x, campos.y, campos.z);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	ModelShader->Use();
	for (int i = 0;i < meshes.size();++i)
		meshes[i].drawMesh(ModelShader->getProgram());
}
GLuint Model::GetModelShaderID() 
{
	return ModelShader->getProgram();
}
void Model::UpdateLightPosition(glm::vec3 &LightPos)

{
	lpos = LightPos;
}








