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
	GLfloat rate = Control::DeltaTime * 40;
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
		
	
	}
	timepassed = glfwGetTime();
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		Control::Pawn->MoveForward(1.0);
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		Control::Pawn->MoveForward(0.0);
	}
}
Control::Control(GLFWwindow *InWindow, Actor *InPawn, SHCamera *InCamera)
{
	Control::Pawn = InPawn;
	Control::Pawn->SetCameraState(true);
	Control::Camera = InCamera;
	Control::Window = InWindow;
	
	glfwSetScrollCallback(Window, Control::scroll_callback);
	glfwSetKeyCallback(Window, Control::key_callback);
	(Window, GLFW_STICKY_KEYS, 1);
}
void Control::SetTarget(Actor* InTarget) {
	
		Target = InTarget;
		
	
}
void Control::SetWindow(GLFWwindow *InWin)
{
	if(InWin)
	Control::Window = InWin;
}
void Control::SetPawn(Actor * InPawn)
{
	static double timepassed;
	double seconds = glfwGetTime();
	if (seconds - timepassed > 0.15) {

			SetTarget(Control::Pawn);
		
			Control::Pawn->SetCameraState(false);
			InPawn->SetCameraState(true);
			Control::Pawn = InPawn;
	}
		timepassed = glfwGetTime();
		
	
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
	rate = Control::DeltaTime * 40;

	Dstate = glfwGetKey(Window, GLFW_KEY_0);
	
	if (glfwGetKey(Window, GLFW_KEY_R))
	{
		if(Target)
		Pawn->ShootRay(Target);
		
	}
	if (glfwGetKey(Window, GLFW_KEY_Q))
	{
		Pawn->StrafeLeft(rate);
	}
	if (glfwGetKey(Window, GLFW_KEY_E))
	{
		Pawn->StrafeRight(rate);
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

