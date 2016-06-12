#include"Model.h"


void Model::SetMeshMode(int In)

{
	meshes[0].SetMode(In);
}

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
		
	switch (shader)
	{
							
	case MaterialType::TEXTURE_2D_DISPLACEMENT:
			
			ModelShader = new Shaders("shaders/PassThrough.vs.glsl", "shaders/PassThrough.tcs.glsl", "shaders/PassThrough.tes.glsl", "shaders/PassThrough.fs.glsl");
			glPatchParameteri(GL_PATCH_VERTICES, 3);
			break;
		case MaterialType::TEXTURE_2D_REFLECT:
			ModelShader = new Shaders("shaders/ReflectVert.glsl", "shaders/ReflectFrag.glsl");
			break;
		case MaterialType::TEXTURE_2D_REFRACT:
			ModelShader = new Shaders("shaders/ReflectVert.glsl", "shaders/RefractFrag.glsl");
			break;
		case MaterialType::TEXTURE_3D:
			ModelShader = new Shaders("shaders/3Dvert.glsl", "shaders/3Dfrag.glsl");
			SkyBox = glGetUniformLocation(ModelShader->getProgram(), "SkyBox");
			std::cout << "\n cubemap shader loaded";
			break;
		case MaterialType::NO_TEXTURE:
			ModelShader = new Shaders("shaders/vert.glsl", "shaders/frag.glsl");
			break;
		default://currently covers all textures through mesh modes and logic in the shader.
			//To difficult to debug, will split shaders. 
			ModelShader = new Shaders("shaders/texVert.glsl", "shaders/texFrag.glsl");

		//TODO add cases for undisplayed cubemap
	}
	
	
	lightColor = glm::vec3(1, 1, 1);
	ProjectionMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Projection");
	ViewMatrixID = glGetUniformLocation(ModelShader->getProgram(), "View");
	ModelMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Model");
	

	lightPosLoc = glGetUniformLocation(ModelShader->getProgram(), "lightPos");
	viewPosLoc = glGetUniformLocation(ModelShader->getProgram(), "viewPos");
	LC = glGetUniformLocation(ModelShader->getProgram(), "lightColor");
	
	
}
void Model::SetViewMatrix(glm::mat4 InView)
{
	//View = InView;
}
Model::~Model()
{
	glDeleteProgram(ModelShader->getProgram());
	delete ModelShader;
}

void Model::drawModel(glm::vec3 &campos,glm::mat4 &Model,glm::mat4 & Projection, glm::mat4 &InView)const 
{	
		
	if (ModelMaterial == MaterialType::TEXTURE_3D)
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		ModelShader->Use();

		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &InView[0][0]);

		for (int i = 0;i < meshes.size();i++)
		{	
			meshes[i].drawMesh(ModelShader->getProgram(), ModelMaterial);
		
		}
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}
	else
	{
		ModelShader->Use();

		glUniform3fv(LC, 1, &lightColor[0]);
		glUniform3f(lightPosLoc, lpos.x, lpos.y, lpos.z);
		glUniform3f(viewPosLoc, campos.x, campos.y, campos.z);
		glUniform1f(SkyBox, 1.0f);
		glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &InView[0][0]);
					
		for (int i = 0;i < meshes.size();i++)
		{
			meshes[i].drawMesh(ModelShader->getProgram(), ModelMaterial);
		}
	}
}
GLuint Model::GetModelShaderID() 
{
	return ModelShader->getProgram();
}
void Model::UpdateLightPosition(glm::vec4 &LightPos)

{
	lpos = glm::vec3(LightPos);
}
void Model::SetActor(glm::vec3 actor)
{
	modelactor = actor;
}







