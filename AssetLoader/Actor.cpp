#include "Actor.hpp"
#include"Transform.h"

Actor::Actor(const std::string & InPath, MeshType MatType, GLFWwindow * InWindow, SHCamera *InCam) : Model(InPath, MatType)
{
	Position = glm::vec3(0, 21, -10);
	window = InWindow;
	Camera = InCam;
	CurrentTerrain = nullptr;
}
Actor::Actor(const std::string & InPath, MeshType MatType,SHCamera *InCam, Terrain *InTerrain, Collider *InCollider) : Model(InPath, MatType)
{
	this->Position = glm::vec3(0, 0, 40);
	Camera = InCam;
	CurrentTerrain = InTerrain;
	this->Position.y = CurrentTerrain->GetHeight(this->Position);
	this->FTarget = this->Position;
	this->CollisionBox = InCollider;
	this->HasCamera = false;
	this->ExternalTransforms = glm::mat4(1.0f);
}
Actor::~Actor()
{
}
void Actor::SetCameraState(bool CameraSelect) {
	this->HasCamera = CameraSelect;
}
void Actor::StrafeLeft(GLfloat rate) {
	FTarget -= glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp())) * rate;
	Position = ActorLerp(Position, FTarget);
}
void Actor::StrafeRight(GLfloat rate) {
	FTarget += glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp())) * rate;
	Position = ActorLerp(Position, FTarget);
}
void Actor::MoveForward(GLfloat rate) {
	glm::vec3 FrontVector = glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp()));
	FTarget -= glm::normalize(glm::cross(FrontVector, Camera->GetCamUp()))*rate;
	Position = ActorLerp(Position, FTarget);
}
void Actor::MoveBackward(GLfloat rate) {
	glm::vec3 BackVector = glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp()));
	FTarget += glm::normalize(glm::cross(BackVector, Camera->GetCamUp()))*rate;
	Position = ActorLerp(Position, FTarget);
}
void Actor::Jump(bool lerp) {
	if (lerp)
	{
		Position.y += 10;
		lerping = lerp;
		std::cout << "\nHop ";
	}
	if (!lerp)
	{

	}
}
glm::vec3 Actor::ActorLerp(glm::vec3 Start, glm::vec3 End) {
	glm::vec3 Current;
	Time = 0.30;
	
		Current.z = Transform::Lerp(Start.z, End.z, Time);
		Current.x = Transform::Lerp(Start.x, End.x, Time);
		Current.y = Transform::Lerp(Start.y, End.y, 0.10);
		
	return Current;
}
void Actor::SetTransforms(glm::mat4 InTransformation) {

	ExternalTransforms = InTransformation;
}
void Actor::UpdateActor()
{
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
		this->ModelMatrix = Transform::translate(this->Position.x, this->Position.y + 5, this->Position.z)*ExternalTransforms;
		this->ModelMatrix = glm::rotate(this->ModelMatrix, Camera->GetYaw(), Camera->GetCamUp());
		Camera->UpdateCamera(this->Position);
	}
	else
	{
		this->ModelMatrix = Transform::translate(this->Position.x, this->Position.y + 5, this->Position.z)*ExternalTransforms;
	}
	this->CollisionBox->SetModelMatrix(this->ModelMatrix);
}
void Actor::Draw() 
{
	glm::vec3 campos = Camera->GetCamPos();
	UpdateActor();
	ModelShader->Use();
	/*glUniform3fv(LC, 1, &lightColor[0]);
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
	Position = InPos;
}