#ifndef ACTOR_H
#define ACTOR_H
#include "Model.h"
//#include<GLFW/glfw3.h>
#include"SHCamera.h"
#include"Terrain.hpp"
#include "Collider.hpp"
#include"GraphicsDebug.h"
#include<memory>


class Actor : public Model
{
	friend class ModelRenderer;

	glm::vec3 Position;
	glm::vec3 RightVector;
	glm::vec3 RayTarget;
	std::unique_ptr<GraphicsDebug> udbg;
	GraphicsDebug* dbg;
	//RenderComponent* Renderer;
	
	Terrain *CurrentTerrain;
	Collider *CollisionBox;

	GLfloat deltaTime, lastFrame, ratemultiplier, Velocity;
	bool lerping;
	bool HasCamera,CameraRotating;
	int Dstate;
	
	
	glm::vec3 FTarget, BTarget, LSideTarget, RSideTarget, NW, NE, SW, SE, CurrentPosition;
	GLfloat Time;

	Renderer* a;

	SHCamera* Camera;
	void UpdateActor();
	glm::mat4 ModelMatrix, ExternalTransforms, CollisionMatrix;
protected:

	
	
	
public:
	Actor(const std::string & InPath, MeshType, SHCamera *InCam, Terrain *InTerrain, Collider* InCollider);
	Actor(const std::string & InPath, MeshType, SHCamera *InCam);
	Actor();
	~Actor();
	void ShootRay(Actor * Target);
	void CheckSphereCollision(Collider * TargetCollider);
	Collider * GetCollider();
	void SetMovement(bool);
	void SetCameraState(bool CameraSelect);
	void SetCamRotate(bool CamRotate);
	void StrafeLeft(GLfloat rate);
	void StrafeRight(GLfloat rate);
	void MoveForward(GLfloat rate);
	void MoveBackward(GLfloat rate);
	void CorrectCollision(glm::vec3 CollisionNormal, GLfloat CollisionDepth);
	void Jump(bool lerp);
	void Draw();
	glm::vec3 GetTargetPosition()const;
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 InPos);

	glm::vec3 ActorLerp(glm::vec3 , glm::vec3 );
	void SetTransforms(glm::mat4 InScale);
};

#endif
