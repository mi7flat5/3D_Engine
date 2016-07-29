#include "CubeMap.hpp"
CubeMap::CubeMap() {}
CubeMap::CubeMap(MeshType shader):Model("assets/box.fbx",shader)
{
	bIsDrawn = true;
	SkyBox = glGetUniformLocation(ModelShader->getProgram(), "SkyBox");
}

void CubeMap::Draw() const
{
	ModelShader->Use();
	glm::mat4 CubeView = glm::mat4(glm::mat3(SHCamera::View));// remove translations
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &CubeView[0][0]);
	if (bIsDrawn)//for environment mapping
	{
		for (int i = 0;i < meshes.size();i++)
		{
			meshes[i].DrawMesh(ModelShader->getProgram(), ModelMaterial);
		}
	}
}

