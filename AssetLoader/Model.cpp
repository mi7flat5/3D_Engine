#include"Model.h"


Model::Model(const std::string &InPath, MeshType Mat)
{
	ModelMaterial = Mat;
	LoadUtility::loadModel(meshes, InPath,Mat);
	IsNormMapping = true;
	DispLevel = 0.0;
	Shininess = 0.2;
	
	switch (Mat)
	{
							
		case MeshType::TEXTURE_2D_DISPLACEMENT:
			ModelShader = new Shaders(  "shaders/PassThrough.vs.glsl", 
										"shaders/PassThrough.tcs.glsl",
										"shaders/PassThrough.tes.glsl", 
										"shaders/PassThrough.fs.glsl");
			
			NormMappingID = glGetUniformLocation(ModelShader->getProgram(), "NormMapping");
			DispLevelID = glGetUniformLocation(ModelShader->getProgram(), "DispLevel");
			SpecularShininessID = glGetUniformLocation(ModelShader->getProgram(), "material.shininess");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
			break;
		case MeshType::TERRAIN:
			/*ModelShader = new Shaders(  "shaders/dispmap.vs.glsl",
									    "shaders/dispmap.tcs.glsl",
									    "shaders/dispmap.tes.glsl",
									    "shaders/dispmap.fs.glsl");*/
			break;
		case MeshType::TEXTURE_2D_REFLECT:
			ModelShader = new Shaders("shaders/ReflectVert.glsl", "shaders/ReflectFrag.glsl");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
			break;
		case MeshType::TEXTURE_2D_REFRACT:
			ModelShader = new Shaders("shaders/ReflectVert.glsl", "shaders/RefractFrag.glsl");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
		
			break;
		case MeshType::SKYBOX:
			ModelShader = new Shaders("shaders/3Dvert.glsl", "shaders/3Dfrag.glsl");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
			break;
		case MeshType::ENVIRONMENT_MAP:
			ModelShader = new Shaders("shaders/3Dvert.glsl", "shaders/3Dfrag.glsl");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
			break;
		case MeshType::NO_TEXTURE:
			ModelShader = new Shaders("shaders/vert.glsl", "shaders/frag.glsl");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
			break;
		default://currently covers all textures through mesh modes and logic in the shader.
			//To difficult to debug, will split shaders. 
			ModelShader = new Shaders("shaders/texVert.glsl", "shaders/texFrag.glsl");
			for (GLuint i = 0; i < meshes.size();i++)
				meshes[i].SetShader(ModelShader->getProgram());
		//TODO add cases for undisplayed cubemap
	}
	
	switch (Mat)
	{
	case (MeshType::TERRAIN):
		break;
	default:		
		lightColor = glm::vec3(1, 1, 1);
		ProjectionMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Projection");
		ViewMatrixID = glGetUniformLocation(ModelShader->getProgram(), "View");
		ModelMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Model");


		lightPosLoc = glGetUniformLocation(ModelShader->getProgram(), "lightPos");
		viewPosLoc = glGetUniformLocation(ModelShader->getProgram(), "viewPos");
		LC = glGetUniformLocation(ModelShader->getProgram(), "lightColor");

	}

}

Model::~Model()
{
	glDeleteProgram(ModelShader->getProgram());
	delete ModelShader;
}

void Model::Draw(glm::vec3 & campos, glm::mat4 & Model)
{
	ModelShader->Use();
	glUniform3fv(LC, 1, &lightColor[0]);
	glUniform1i(NormMappingID, IsNormMapping);
	glUniform1f(SpecularShininessID, Shininess);
	glUniform1f(DispLevelID, 5);
	glUniform3f(lightPosLoc, lpos.x, lpos.y, lpos.z);
	glUniform3f(viewPosLoc, campos.x, campos.y, campos.z);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &SHCamera::View[0][0]);

	for (int i = 0;i < meshes.size();i++)
	{
		meshes[i].DrawMesh(ModelShader->getProgram(), ModelMaterial);
	}

}
void Model::SetPosition(glm::vec3 InPos) {
	Position = InPos;
}
void Model::SetScale(glm::mat4 InScale) {
	;
}
void Model::SetShaders(Shaders* InShader) {
	ModelShader = InShader;

}







