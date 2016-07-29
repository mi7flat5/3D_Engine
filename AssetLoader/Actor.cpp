#include "Actor.hpp"
#include"Transform.h"
#include"Control.hpp"
#include"glm-0.9.2.7\glm\gtx\quaternion.hpp"

//#include"glm-0.9.2.7\glm\gtc\quaternion.hpp>

#include"ActorComponents.hpp"


Actor::Actor(const std::string & InPath, MeshType MatType,  SHCamera *InCam) : Model(InPath, MatType)
{
	Position = glm::vec3(0, 0, -10);
	
	Camera = InCam;
	CurrentTerrain = nullptr;
}
Actor::Actor()
{
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
	this->dbg = nullptr;
	ratemultiplier = 5.0;
	Velocity = 0.0;
	a =  new ModelRenderer();
}
Actor::~Actor()
{
	delete a;
}

void Actor::ShootRay(Actor* Target) {
	
	vec3 ModelPosition = vec3(ModelMatrix[3]);
	RayTarget = FTarget;
	RayTarget += glm::normalize(glm::cross(RightVector, vec3(0,1,0)))*30.0f;
	RayTarget.y = CurrentTerrain->GetHeight(Target->GetPosition())+5;
	/*RayTarget = FTarget;
	RayTarget += glm::normalize(ModelPosition - Camera->GetCamPos())*30.0f;*/
	
	
	if (Target->GetCollider()->SphereRayCollider(ModelPosition, glm::normalize(RayTarget - ModelPosition)))
		{Target->Jump(true);
		//Target->SetPosition(RayTarget);
	Target->CorrectCollision(glm::normalize(RayTarget - ModelPosition)*7.0f,1.0f);
	}
	
	std::vector<glm::vec3> tmp;
	tmp.push_back(ModelPosition);
	tmp.push_back(RayTarget);
	dbg =  new GraphicsDebug(tmp,dbgType::LINE);
	
	
	
}

void Actor::CheckSphereCollision(Collider* TargetCollider) {
	if (CollisionBox->SphereSphereCollider(TargetCollider))
		std::cout << "\nBounding sphere collision. ";
}

Collider* Actor::GetCollider() { return this->CollisionBox; }

void Actor::SetMovement(bool lerp) { lerping = lerp; }

void Actor::SetCameraState(bool CameraSelect) {
	this->HasCamera = CameraSelect;
}
void Actor::StrafeLeft(GLfloat rate) {
	FTarget -= glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp())) * rate;
	
}
void Actor::StrafeRight(GLfloat rate) {
	FTarget += glm::normalize(glm::cross(Camera->GetCamFront(), Camera->GetCamUp())) * rate;
	
}
void Actor::MoveForward(GLfloat rate) {
	
	
		/*FTarget += glm::normalize(glm::cross(FrontVector, Camera->GetCamUp()))*rate;*/
	Velocity =rate;
	
	
	
	
}
void Actor::MoveBackward(GLfloat rate) {
	

	FTarget -= glm::normalize(glm::cross(RightVector, Camera->GetCamUp()))*rate;
	
}
void Actor::CorrectCollision(glm::vec3 CollisionNormal, GLfloat CollisionDepth) {
	
	
	if (!isnan(CollisionNormal.x)) {
		if (glm::cross(glm::normalize(FTarget - Position), CollisionNormal).y > 0)
		{
			
			//FTarget += glm::normalize(glm::cross(CollisionNormal, Camera->GetCamUp()))*Control::DeltaTime*20.0f;
			FTarget += CollisionNormal*Control::DeltaTime*180.0f;
		}
		else
		{
			
			//FTarget -= glm::normalize(glm::cross(CollisionNormal, Camera->GetCamUp()));
			FTarget += CollisionNormal*Control::DeltaTime*180.0f;
		}
		
		
	
			
	}
}
void Actor::Jump(bool lerp) {
	std::cout << "\nTarget hit!";
}
glm::vec3 Actor::ActorLerp(glm::vec3 Start, glm::vec3 End) {
	glm::vec3 Current;
	if(HasCamera)
	Time = 0.30;
	else Time = 0.05;
	
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
	RightVector = Camera->GetRight();
	FTarget += Camera->GetXZfront()*Velocity*Control::DeltaTime*40.0f;
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

	UpdateActor();
	a->render(this);

	//glm::vec3 campos = Camera->GetCamPos();
	//UpdateActor();
	//ModelShader->Use();
	//glUniform3fv(LC, 1, &lightColor[0]);/*
	//glUniform1i(NormMappingID, IsNormMapping);
	//glUniform1f(SpecularShininessID, Shininess);
	//glUniform1f(DispLevelID, DispLevel);*/
	//glUniform3f(lightPosLoc, lpos.x, lpos.y, lpos.z);
	//glUniform3f(viewPosLoc, campos.x, campos.y, campos.z);
	//glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
	//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &SHCamera::View[0][0]);

	//for (int i = 0;i < meshes.size();i++)
	//{
	//	this->meshes[i].DrawMesh(ModelShader->getProgram(), ModelMaterial);
	//}
	//CollisionBox->Draw();
	if (dbg)
		dbg->Draw(ModelMatrix);

	delete dbg;
	dbg = nullptr;


}
glm::vec3 Actor::GetTargetPosition()const {

	return FTarget;
}
glm::vec3 Actor::GetPosition()const {

	return Position;
}
void Actor::SetPosition(glm::vec3 InPos) {
	
	InPos.y = CurrentTerrain->GetHeight(InPos);
	
	
	FTarget = InPos;
	//Position = InPos;

}

