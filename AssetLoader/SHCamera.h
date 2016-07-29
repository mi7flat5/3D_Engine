#ifndef SHCAMERA_H
#define SHCAMERA_H
#include"include\GL\glew.h"

#include"glm-0.9.2.7\glm\glm.hpp"
#include"glm-0.9.2.7\glm\gtc\matrix_transform.hpp"
#include"glm-0.9.2.7\glm\gtc\type_ptr.hpp"

class SHCamera
{
	GLfloat Xoffset, Zoffset, vdist, hdist;
	GLfloat yaw, pitch, theta, phi;
	GLfloat TerrainHeight;
	GLfloat FieldOfView;
	
	GLuint WIDTH, HEIGHT;
	
	glm::vec3 front, campos, camfront, camup, Right,Target,Home;
	
	glm::mat4 CurrentView;
	
	void SHCamera::UpdateOffsetsVectors();

	

	
		
	bool TargSelect;
	bool Moving;

public:
	
	static GLfloat radius;
	glm::vec3 MovePosition(glm::vec3 Start, glm::vec3 end);
	glm::vec3 MovePositionReverse(glm::vec3 Start, glm::vec3 end);
	SHCamera(GLuint Width, GLuint Height);
	~SHCamera();
	glm::vec3 GetXZfront();
	
	glm::vec3 GetHome() const;
	void SwithTarget();
	bool IsMoving();
	glm::mat4 UpdateCamera(  glm::vec3&);
	void SetYaw(GLfloat InYaw);
	glm::mat4 UpdateCamera();
	glm::mat4 GetView()const;
	GLfloat GetPitch() const;
	void SetCamUp(glm::vec3 InUp);
	void SetTerrainHeight(GLuint);
	glm::vec3 GetCamPos()const;
	glm::vec3 GetCamFront()const;
	glm::vec3 GetCamUp()const;
	GLfloat GetYaw()const;
	void SetHome(glm::vec3&);
	void SetPitch(GLfloat InPitch);
	glm::vec3 GetRight();
	static glm::mat4 Projection;
	static glm::mat4 View;
	
};
#endif

