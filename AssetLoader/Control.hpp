#ifndef CONTROL_HPP
#define CONTROL_HPP

#include"Actor.hpp"
#include"include/GL/glew.h"
#include"include/GLFW/glfw3.h"
#include"include/GL/glut.h"
#include"SHCamera.h"

class Control
{
	static GLFWwindow* Window;
	static Actor* Pawn;
	static SHCamera* Camera;

	Actor* Target;
	
	GLfloat lastX, lastY,lastFrame,rate;
	GLdouble TimePassed;
	GLint Dstate;
	
public:
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow*, int, int, int, int);
	static GLfloat DeltaTime;
	Control(GLFWwindow * InCurrentWindow, Actor * InCurrentActor, SHCamera *InCamera);
	void SetWindow(GLFWwindow *);
	void SetPawn(Actor *);
	void SetCamera(SHCamera *);
	~Control();
	void UpdateControls();
	void SetTarget(Actor* InTarget);
	
};


#endif // !CONTROL_HPP
