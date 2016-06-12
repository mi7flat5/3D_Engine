#pragma once
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
class SHCamera
{
	GLfloat Xoffset;
	GLfloat Zoffset;
	
	GLfloat vdist;
	GLfloat hdist;
	
	GLfloat yaw;
	GLfloat pitch;
	GLfloat theta;
	GLfloat phi;

	GLfloat lastX;
	GLfloat lastY;

	glm::vec3 front;
	glm::vec3 campos;
	glm::vec3 camfront;
	glm::vec3 camup;

	glm::mat4 CurrentView;
	glm::vec3 Right;

	void SHCamera::UpdateOffsetsVectors();
	
public:
	
	static GLfloat radius;
	
	SHCamera();
	~SHCamera();
	glm::mat4 UpdateCamera(GLFWwindow*,  glm::vec3&);
	glm::mat4 UpdateCamera();
	glm::mat4 GetView()const;
	GLfloat GetPitch() const;
	void SetCamUp(glm::vec3 InUp);
	glm::vec3 GetCamPos()const;
	glm::vec3 GetCamFront()const;
	glm::vec3 GetCamUp()const;
	GLfloat GetYaw()const;
};


