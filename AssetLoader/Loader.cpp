#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/glut.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include"Model.h"
#include"Transform.h"
#include"SHCamera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void moveActor(GLFWwindow* );

GLuint WIDTH = 1200;
GLuint HEIGHT = 1600;
GLfloat fov = 90.0f;

glm::vec3 actor(0, 0, 0);

glm::vec3 lpos(0, 10,  20);
glm::vec3 lightColor(1, 1, 1);

SHCamera cam;
GLfloat SHCamera::radius;

int main() { 
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Loader", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	glViewport(0,0, HEIGHT, WIDTH);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	

	Model myMeshes("assets/ground.fbx", "shaders/texVert.glsl", "shaders/texFrag.glsl");
	Model myMeshes2("assets/SkySphere.fbx","shaders/texVert.glsl", "shaders/texFrag.glsl");
	Model myMeshes3("assets/AY.fbx", "shaders/texVert.glsl", "shaders/texFrag.glsl");
		
	glm::mat4 Model;
	glm::mat4 Projection = glm::perspective(fov, (float)HEIGHT / WIDTH, 0.1f, 300.0f);
	glm::mat4 MVP = Projection * Model;
	
	while (!glfwWindowShouldClose(window)) 
		{
			glm::mat4 Projection = glm::perspective(fov, (GLfloat)HEIGHT / WIDTH, 0.1f, 300.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_BACK);

			
			//anna
			Model = cam.UpdateCamera(window,actor)*Transform::translate(actor.x, actor.y, actor.z);
			Model = glm::rotate(Model,cam.GetYaw(),cam.GetCamUp()) *Transform::scale(.01,.01,.01);
			myMeshes.drawModel(cam.GetCamPos(),Model,MVP);
			
			//ground			
			Model = cam.UpdateCamera(window, actor)*Transform::scale(200, 200, 200);
			myMeshes2.drawModel(cam.GetCamPos(), Model, MVP);
			//sky
			Model = cam.UpdateCamera(window, actor)*Transform::translate(actor.x, actor.y, actor.z)*Transform::scale(200, 200, 200);
			myMeshes3.drawModel(cam.GetCamPos(), Model, MVP);
					
			
			moveActor(window);
			myMeshes.UpdateLightPosition(lpos);//hack fix later
			myMeshes2.UpdateLightPosition(lpos);
			myMeshes3.UpdateLightPosition(lpos);
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		
	glfwTerminate();
	
	return 0; 
}
/*User control, callbacks*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		// When a user presses the escape key, we set the WindowShouldClose property to true, 
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_KP_8)
			lpos.z = lpos.z + 100;
		if (key == GLFW_KEY_KP_2)
			lpos.z = lpos.z - 100;
		if (key == GLFW_KEY_KP_6)
			lpos.x = lpos.x + 100;
		if (key == GLFW_KEY_KP_4)
			lpos.x = lpos.x - 100;
		if (key == GLFW_KEY_KP_ADD)
			lpos.y = lpos.y + 100;
		if (key == GLFW_KEY_KP_SUBTRACT)
			lpos.y = lpos.y - 100;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
		if (SHCamera::radius >= 2.0f && SHCamera::radius <= 50.0f)
			SHCamera::radius += yoffset;
		else
			SHCamera::radius -= yoffset;
}
void moveActor(GLFWwindow*window)
{
	float rate = .65;
	if (glfwGetKey( window, GLFW_KEY_D))
	{
		actor += glm::normalize(glm::cross(cam.GetCamFront(), cam.GetCamUp())) *rate;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		actor -= glm::normalize(glm::cross(cam.GetCamFront(), cam.GetCamUp())) * rate;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		glm::vec3 FrontVector = glm::normalize(glm::cross(cam.GetCamFront(), cam.GetCamUp()));
		actor -= glm::normalize(glm::cross(FrontVector, cam.GetCamUp()))*rate;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		glm::vec3 BackVector = glm::normalize(glm::cross(cam.GetCamFront(), cam.GetCamUp()));
		actor += glm::normalize(glm::cross(BackVector, cam.GetCamUp()))*rate;
	}
	
	actor.y = 0;
}