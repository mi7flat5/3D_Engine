#include "Collider.hpp"
#include"SHCamera.h"
#include"LoadUtility.h"
#include"Transform.h"

int Collider::PointsInCollider()
{
	return Verts.size();
}
void Collider::SetModelMatrix(glm::mat4 InMat)
{
	ColliderModel = InMat*Transform::scale(1,1,1);
}
Collider::Collider(const std::string &InPath)
{
	LoadUtility::LoadCollider(InPath, Verts,Indices,RenderVerts);
	ColliderModel = glm::mat4(1.0f);
	ModelShader = new Shaders("shaders/ColliderVert.glsl", "shaders/ColliderFrag.glsl");
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, RenderVerts.size()*sizeof(glm::vec3), &RenderVerts[0], GL_STATIC_DRAW);
			
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);//indices
	glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (GLvoid*)0);

	ProjectionMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Projection");
	ViewMatrixID = glGetUniformLocation(ModelShader->getProgram(), "View");
	ModelMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Model");
	glPointSize(4.0f);
	glLineWidth(10.0f);
}
Collider::~Collider()
{

}
Collider::Collider() {}

void Collider::Draw()
{
	glBindVertexArray(this->VAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ModelShader->Use();
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ColliderModel[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &SHCamera::View[0][0]);
	glDrawElements(GL_LINES, this->Indices.size(), GL_UNSIGNED_INT, 0);
	
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}
glm::mat4 Collider::GetColliderMatrix() const
{
	return ColliderModel;
}
glm::vec3 Collider::GetFurthestPoint(const glm::vec3 &DirectionVector)
{
	glm::vec3 WorldVertex = glm::vec3(ColliderModel * glm::vec4(Verts[0].x, Verts[0].y, Verts[0].z, 1));

	GLfloat MaxDot = glm::dot(WorldVertex, DirectionVector);
	glm::vec3 FurthestPoint = WorldVertex;

	for (int i = 1; i < Verts.size(); i++)
	{
		WorldVertex = glm::vec3(ColliderModel * glm::vec4(Verts[i].x, Verts[i].y, Verts[i].z, 1));

		GLfloat Dot = glm::dot(WorldVertex, DirectionVector);
		if (Dot > MaxDot)
		{
			MaxDot = Dot;
			FurthestPoint = WorldVertex;
		}
	}
	return FurthestPoint;
}