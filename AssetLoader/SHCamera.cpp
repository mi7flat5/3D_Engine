#include "SHCamera.h"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

SHCamera::SHCamera () : pitch(30), campos(glm::vec3(0, 0, 5)),camfront(glm::vec3(0, 0, -1)),camup(glm::vec3(0, 1, 0))
{
	radius = 20.0;
	vdist = radius*sin(glm::radians(pitch));
}
SHCamera::~SHCamera(){}
glm::mat4 SHCamera::UpdateCamera(GLFWwindow *window,const glm::vec3 &actor)
{
	double xpos, ypos;
	static int FirstRun = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || FirstRun == 0)
	{
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.7;	// Change this value to your liking
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
		front.y = radius * cos(glm::radians(pitch + 90));
		front.z = radius * sin(glm::radians(phi))*sin(glm::radians(theta));
		camfront = glm::normalize(front);


		vdist = radius*sin(glm::radians(pitch));
		GLfloat hdist = radius*cos(glm::radians(pitch));
		Xoffset = hdist*sin(glm::radians(yaw));
		Zoffset = hdist*cos(glm::radians(yaw));
		if(FirstRun==0)//hack fix later
		++FirstRun;
	}
	campos.x = actor.x - Xoffset;
	campos.y = vdist + 10;
	campos.z = actor.z - Zoffset;
	lastX = xpos;
	lastY = ypos;
	return glm::lookAt(campos, campos + camfront, camup);
}
void SHCamera::UpdateCamera() {}

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
