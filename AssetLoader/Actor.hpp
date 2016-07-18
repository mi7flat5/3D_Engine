#ifndef ACTOR_H
#define ACTOR_H
#include "Model.h"
#include<GLFW/glfw3.h>
#include"SHCamera.h"
#include"Terrain.hpp"
#include "Collider.hpp"
class Actor : protected Model
{
	glm::vec3 Position;
	glm::vec3 FrontVector;
	
	SHCamera* Camera;
	Terrain *CurrentTerrain;
	Collider *CollisionBox;
	glm::mat4 ModelMatrix,ExternalTransforms,CollisionMatrix;
	GLfloat deltaTime, lastFrame;
	bool lerping;
	bool HasCamera,CameraRotating;
	int Dstate;
	void UpdateActor();
	
	glm::vec3 FTarget, BTarget, LSideTarget, RSideTarget, NW, NE, SW, SE, CurrentPosition;
	GLfloat Time;
public:
	Actor(const std::string & InPath, MeshType, SHCamera *InCam, Terrain *InTerrain, Collider* InCollider);
	Actor(const std::string & InPath, MeshType, SHCamera *InCam);

	~Actor();
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
