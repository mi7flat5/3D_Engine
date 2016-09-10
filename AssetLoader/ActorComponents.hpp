#pragma once
#include"Actor.hpp"
class Renderer 
{

	
public:
	
	virtual ~Renderer() {};
	virtual void render(Actor* a) = 0;
	
	
	
};
class ModelRenderer : public Renderer
{
public:
	//ModelRenderer() {}
	
	virtual void render(Actor* a){
		glm::vec3 campos = a->Camera->GetCamPos();
		
		a->ModelShader->Use();
		glUniform3fv(a->LC, 1, &a->lightColor[0]);/*
		glUniform1i(NormMappingID, IsNormMapping);
		glUniform1f(SpecularShininessID, Shininess);
		glUniform1f(DispLevelID, DispLevel);*/
		glUniform3f(a->lightPosLoc, a->lpos.x, a->lpos.y, a->lpos.z);
		glUniform3f(a->viewPosLoc, campos.x, campos.y, campos.z);
		glUniformMatrix4fv(a->ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
		glUniformMatrix4fv(a->ModelMatrixID, 1, GL_FALSE, &a->ModelMatrix[0][0]);
		glUniformMatrix4fv(a->ViewMatrixID, 1, GL_FALSE, &SHCamera::View[0][0]);

		for (int i = 0;i < a->meshes.size();i++)
		{
			a->meshes[i].DrawMesh(a->ModelShader->getProgram(), a->ModelMaterial);
		}

	}

};
