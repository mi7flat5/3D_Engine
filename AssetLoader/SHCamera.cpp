#include "SHCamera.h"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<iostream>

SHCamera::SHCamera () : pitch(0), campos(glm::vec3(0, 0, 5)),camfront(glm::vec3(0, 0, -1)),camup(glm::vec3(0, 1, 0)),yaw(0)
{
	radius = 70.0;
	vdist = radius*sin(glm::radians(pitch));
}
SHCamera::~SHCamera(){}
glm::mat4 SHCamera::UpdateCamera(GLFWwindow *window,const glm::vec3 &actor)
{
	double xpos, ypos;
	static int FirstRun = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || FirstRun == 0|| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))//TODO move controls out of class, add this conditional to its own method
	{
		
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.7;	// TODO make prameter for sensitivity
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89)
			pitch = -89;

		glm::vec3 front;
		GLfloat theta = 90 - yaw;
		GLfloat phi = 90 - pitch;

		front.x = radius * sin(glm::radians(phi))*cos(glm::radians(theta));
		front.y = radius * cos(glm::radians(pitch + 90)); //TODO add parameter for adjusting y facing offset
		front.z = radius * sin(glm::radians(phi))*sin(glm::radians(theta));
		camfront = glm::normalize(front);


		vdist = radius*sin(glm::radians(pitch));
		GLfloat hdist = radius*cos(glm::radians(pitch));
		Xoffset = hdist*sin(glm::radians(yaw));
		Zoffset = hdist*cos(glm::radians(yaw));
		if(FirstRun==0)//TODO hack fix later
		++FirstRun;
	}
	campos.x = actor.x - Xoffset;
	/*if (vdist < -10)
		vdist = -10;*/
	campos.y = vdist;//TODO add parameter for vertical distance offset
	campos.z = actor.z - Zoffset;
	lastX = xpos;
	lastY = ypos;
	CurrentView = glm::lookAt(campos, campos+camfront, camup);//Variable to access current view for skybox
	/////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!camra lookat changed
	/*glm::vec3 right = glm::normalize(glm::cross(camfront, glm::vec3(0, 1, 0)));
	camup = glm::normalize(glm::cross(right, camfront));*/
	return CurrentView;
}
glm::mat4 SHCamera::UpdateCamera() {
	
	
	return glm::lookAt(campos, campos + camfront, camup); 
}

glm::vec3 SHCamera::GetCamPos()const
{
	return campos;
}

glm::vec3 SHCamera::GetCamFront()const
{
	return camfront;
}

glm::vec3 SHCamera::GetCamUp()const
{
	return camup;
}

GLfloat SHCamera::GetYaw() const
{
	return yaw;
}
glm::mat4 SHCamera::GetView() const
{
	return glm::mat4(glm::mat3(CurrentView));
}
GLfloat SHCamera::GetPitch() const
{
	return pitch;
}