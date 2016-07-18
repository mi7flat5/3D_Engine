#include "Actor.hpp"
#include"Transform.h"
#include"Control.hpp"
#include"glm-0.9.2.7\glm\gtx\quaternion.hpp"
//#include"glm-0.9.2.7\glm\gtc\quaternion.hpp>
Actor::Actor(const std::string & InPath, MeshType MatType,  SHCamera *InCam) : Model(InPath, MatType)
{
	Position = glm::vec3(0, 0, -10);
	
	Camera = InCam;
	CurrentTerrain = nullptr;
}
Actor::Actor(const std::string & InPath, MeshType MatType,SHCamera *InCam, Terrain *InTerrain, Collider *InCollider) : Model(InPath, MatType)
{
	this->Position = glm::vec3(0, 5, 40);
	Camera = InCam;
	CurrentTerrain = InTerrain;
	this->Position.y = CurrentTerrain->GetHeight(this->Position)+5;
	this->FTarget = this->Position;
	this->CollisionBox = InCollider;
	this->HasCamera = false;
	this->ExternalTransforms = glm::mat4(1.0f);
	this->CameraRotating = true;
	
}
Actor::~Actor()
{
}
void Actor::SetMovement(bool lerp) { lerping = lerp; }
void Actor::SetCameraState(bool CameraSelect) {
	this->HasCamera = CameraSelect;
}
void Actor::StrafeLeft(GLfloat rate) {
	FTarget -= glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp())) * rate;
	//Position = ActorLerp(Position, FTarget);
}
void Actor::StrafeRight(GLfloat rate) {
	FTarget += glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp())) * rate;
	//Position = ActorLerp(Position, FTarget);
}
void Actor::MoveForward(GLfloat rate) {
	
		FrontVector = glm::normalize(glm::cross(Camera->GetCamUp(), Camera->GetCamFront()));
		FTarget += glm::normalize(glm::cross(FrontVector, Camera->GetCamUp()))*rate;
	
	//Position = ActorLerp(Position, FTarget);
}
void Actor::MoveBackward(GLfloat rate) {
	
	FrontVector = glm::normalize(glm::cross(Camera->GetCamUp(), Camera->GetCamFront()));
	FTarget -= glm::normalize(glm::cross(FrontVector, Camera->GetCamUp()))*rate;
	//Position = ActorLerp(Position, FTarget);
}
void Actor::CorrectCollision(glm::vec3 CollisionNormal, GLfloat CollisionDepth) {
	
	
	if (!isnan(CollisionNormal.x)) {
		if (glm::cross(glm::normalize(FTarget - Position), CollisionNormal).y > 0)
		{
			
			//FTarget += glm::normalize(glm::cross(CollisionNormal, Camera->GetCamUp()))*Control::DeltaTime*20.0f*CollisionDepth/2.0f;
			FTarget += CollisionNormal/2.0f + CollisionDepth / 2.0f;;
		}
		else
		{
			
			//FTarget -= glm::normalize(glm::cross(CollisionNormal, Camera->GetCamUp()))*Control::DeltaTime*20.0f * CollisionDepth / 2.0f;
			FTarget += CollisionNormal/2.0f+CollisionDepth/2.0f;
		}
		
		
	
			
	}
}
void Actor::Jump(bool lerp) {
	if (lerp)
	{
		/*Position.y += Control::DeltaTime*4000.0f;
		lerping = lerp;
		std::cout << "\nHop ";*/
	}
	if (!lerp)
	{
		lerping = lerp;
	}
}
glm::vec3 Actor::ActorLerp(glm::vec3 Start, glm::vec3 End) {
	glm::vec3 Current;
	Time = 0.30;
	
		Current.z = Transform::Lerp(Start.z, End.z, Time);
		Current.x = Transform::Lerp(Start.x, End.x, Time);
		if(!lerping)
		Current.y = Transform::Lerp(Start.y, End.y, 0.10);
		
	return Current;
}
void Actor::SetTransforms(glm::mat4 InTransformation) {

	ExternalTransforms = InTransformation;
}
void Actor::UpdateActor()
{
	Position = ActorLerp(Position, FTarget);
	if (CurrentTerrain)
	{
		if (FTarget.x > CurrentTerrain->GetMaxX())
			FTarget.x = CurrentTerrain->GetMaxX();
		if (FTarget.z > CurrentTerrain->GetMaxZ())
			FTarget.z = CurrentTerrain->GetMaxZ();
		if (FTarget.x < CurrentTerrain->GetMinX())
			FTarget.x = CurrentTerrain->GetMinX();
		if (FTarget.z < CurrentTerrain->GetMinZ())
			FTarget.z = CurrentTerrain->GetMinZ();
		
			FTarget.y = CurrentTerrain->GetHeight(FTarget);

			
			
		
		Camera->SetTerrainHeight(CurrentTerrain->GetHeight(Camera->GetCamPos()));
	}
	if (this->HasCamera)
	{
		this->ModelMatrix = Transform::translate(this->Position.x, this->Position.y+5 , this->Position.z)*ExternalTransforms;
		if(CameraRotating)
		this->ModelMatrix = glm::rotate(this->ModelMatrix, Camera->GetYaw(), Camera->GetCamUp());
	
		Camera->UpdateCamera(this->Position);
		
	}
	else
	{
		this->ModelMatrix = Transform::translate(this->Position.x, this->Position.y + 5, this->Position.z)*ExternalTransforms;
		
	}
	this->CollisionBox->SetModelMatrix(ModelMatrix);
}
void Actor::SetCamRotate(bool CamRotate) {
	CameraRotating = CamRotate;
}
void Actor::Draw() 
{
	glm::vec3 campos = Camera->GetCamPos();
	UpdateActor();
	ModelShader->Use();
	glUniform3fv(LC, 1, &lightColor[0]);/*
	glUniform1i(NormMappingID, IsNormMapping);
	glUniform1f(SpecularShininessID, Shininess);
	glUniform1f(DispLevelID, DispLevel);*/
	glUniform3f(lightPosLoc, lpos.x, lpos.y, lpos.z);
	glUniform3f(viewPosLoc, campos.x, campos.y, campos.z);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &SHCamera::View[0][0]);

	for (int i = 0;i < meshes.size();i++)
	{
		this->meshes[i].DrawMesh(ModelShader->getProgram(), ModelMaterial);
	}
}
glm::vec3 Actor::GetTargetPosition()const {

	return FTarget;
}
glm::vec3 Actor::GetPosition()const {

	return Position;
}
void Actor::SetPosition(glm::vec3 InPos) {
	
	//InPos.y = CurrentTerrain->GetHeight(InPos);
	
	
	FTarget = InPos;
	Position = InPos;

}