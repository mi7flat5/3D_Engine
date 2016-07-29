#include "Collider.hpp"
#include"SHCamera.h"
#include"LoadUtility.h"
#include"Transform.h"
#include"GraphicsDebug.h"
#define SQR(x) x*x
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

	

	glPointSize(20.0f);
	glLineWidth(10.0f);

	SphereRadius = 2;
}
Collider::~Collider()
{/*
	if (ModelShader)
		delete ModelShader;
	if (DebuglineShader)
		delete DebuglineShader;*/
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
//Actor Position, Actor FrontVector or camera front vector, expects normailed direction
bool Collider::SphereRayCollider(glm::vec3 RayStart,glm::vec3 RayDirection) {
	
	glm::vec3 ThisPosition = glm::vec3(ColliderModel[3]);
	glm::vec3 e = ThisPosition - RayStart;
	
	GLfloat a = glm::dot(e, RayDirection);
	if (a < 0)
		return false;
	
	if (glm::dot(e, e) < SQR(SphereRadius))
		return true;
	
	glm::vec3 Reach = RayDirection*30.0f;

	GLfloat eMagsqr = SQR(e.x) + SQR(e.y) + SQR(e.z);
	GLfloat reachMagsqr = SQR(Reach.x) + SQR(Reach.y) + SQR(Reach.z);
	
	if (eMagsqr > reachMagsqr)
		return false;
		
	if ( (SQR(SphereRadius) - glm::dot(e,e) + SQR(a)) > 0)
		return true;
	
	
	
	return false;
}

//Static sphere collision, no time constraint, called each frame
bool Collider::SphereSphereCollider(Collider* TargetCollider) {
	
	GLfloat DistanceSqr = glm::dot(GetPosition() - TargetCollider->GetPosition(),
								   GetPosition() - TargetCollider->GetPosition());
	
	if (DistanceSqr < SQR(SphereRadius + TargetCollider->GetRadius()))
		return true;

	return false;
}

GLuint Collider::GetRadius()const { return SphereRadius; }

//May need to divide by w of vec4 of ColliderModel[3] DEBUG HERE IF POSITION PROBLEMS
glm::vec3 Collider::GetPosition()const {
	glm::vec4 Wposition = ColliderModel[3];
	if (Wposition.w != 1)
		return glm::vec3(Wposition) / Wposition.w;

	else return glm::vec3(ColliderModel[3]);
}
glm::mat4 Collider::GetColliderMatrix() const
{
	return ColliderModel;
}