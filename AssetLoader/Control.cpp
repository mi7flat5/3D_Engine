#include"Control.hpp"

void Control::scroll_callback(GLFWwindow* Window, double xoffset, double yoffset)
{
	if (SHCamera::radius >= 2.0f && SHCamera::radius <= 200.0f)
	{
		SHCamera::radius += yoffset;
	}
	else if (SHCamera::radius > 2.0f)
		SHCamera::radius -= yoffset;
	else if (SHCamera::radius < 2.0f)
		SHCamera::radius = 2.0f;

}
void Control::key_callback(GLFWwindow* Window, int key, int scancode, int action, int mode)
{
	static double timepassed;
	double seconds = glfwGetTime();
	GLint Spacestate = glfwGetKey(Window, GLFW_KEY_SPACE);
	if (seconds - timepassed > 0.15) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(Window, GL_TRUE);
		if (glfwGetKey(Window, GLFW_KEY_0))
		{	if(!Control::Camera->IsMoving())
				Control::Camera->SwithTarget();
		}
		if (key == GLFW_KEY_SPACE&& action == GLFW_PRESS)
			Pawn->Jump(true);
		else if (Spacestate == GLFW_RELEASE) Pawn->Jump(false);
	}
	timepassed = glfwGetTime();
}
Control::Control(GLFWwindow *InWindow, Actor *InPawn, SHCamera *InCamera)
{
	Control::Pawn = InPawn;
	Control::Pawn->SetCameraState(true);
	Control::Camera = InCamera;
	Control::Window = InWindow;
	
	glfwSetScrollCallback(Window, Control::scroll_callback);
	glfwSetKeyCallback(Window, Control::key_callback);
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, 1);
}
void Control::SetWindow(GLFWwindow *InWin)
{
	if(InWin)
	Control::Window = InWin;
}
void Control::SetPawn(Actor * InPawn)
{
	if (InPawn)
	{
		if (!Control::Camera->IsMoving()) {
					
			Control::Pawn->SetCameraState(false);
			Control::Pawn = InPawn;
			Control::Pawn->SetCameraState(true);
		}
	}
}

void Control::SetCamera(SHCamera * InCam)
{
	if(InCam)
	Control::Camera = InCam;
}
Control::~Control()
{
}
void Control::UpdateControls() {
	GLfloat currentFrame = glfwGetTime();
	Control::DeltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	GLfloat rate = Control::DeltaTime * 40;

	Dstate = glfwGetKey(Window, GLFW_KEY_0);
		
	if (glfwGetKey(Window, GLFW_KEY_Q))
	{
		Pawn->StrafeLeft(rate);
	}
	if (glfwGetKey(Window, GLFW_KEY_E))
	{
		Pawn->StrafeRight(rate);
	}
	if (glfwGetKey(Window, GLFW_KEY_W))
	{
		Pawn->MoveForward(rate);
	}
	if (glfwGetKey(Window, GLFW_KEY_S))
	{
		Pawn->MoveBackward(rate);
	}
	if (glfwGetKey(Window, GLFW_KEY_D))
	{
		Control::Camera->SetYaw(-rate * 2);
	}
	if (glfwGetKey(Window, GLFW_KEY_A))
	{
		Control::Camera->SetYaw(rate * 2);
	}
	double xpos, ypos;
	glfwGetCursorPos(Window, &xpos, &ypos);
	
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT))
	{

		GLfloat xoffset = lastX - xpos;
		GLfloat yoffset = ypos - lastY; 
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.7;	// TODO make variable for sensitivity
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		Control::Camera->SetYaw(xoffset);
		Control::Camera->SetPitch(yoffset);
	}
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT))
	{

		GLfloat xoffset = lastX - xpos;
		GLfloat yoffset = ypos - lastY;
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.7;	// TODO make variable for sensitivity
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		Control::Camera->SetYaw(xoffset);
		Control::Camera->SetPitch(yoffset);
		Pawn->SetCamRotate(false);
	}
	else Pawn->SetCamRotate(true);

	lastX = xpos;
	lastY = ypos;
	
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) && glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		Pawn->MoveForward(rate);
	}
}

