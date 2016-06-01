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

glm::vec3 eye(0, 0, 1);
glm::vec3 center(1, 0, 0);
glm::vec3 up(0, 1, 0);

glm::vec3 actor(0, 0, 0);

glm::vec3 lpos(0, 20,  0);
glm::vec3 lightColor(1, 1, 1);

SHCamera cam;
SHCamera cam1;
SHCamera cam2;
GLfloat SHCamera::radius;
int ProjectionY;
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
	glCullFace(GL_BACK);
	
	Model Character("assets/sphere.fbx", MaterialType::TEXTURE_2D_REFRACT);
	Model Ground("assets/ground.fbx", MaterialType::TEXTURE_2D);
	Model Sky("assets/box.fbx", MaterialType::TEXTURE_3D);
		//*Transform::scale(.02,.02,.02)
	glm::mat4 Model;
	glm::mat4 CharMod(1.0f);
	glm::mat4 GroundMod(1.0f);
	//glm::mat4 SkyMod(1.0f);
	glm::mat4 SkyMod = cam.GetView();
	SkyMod = glm::rotate(SkyMod, 180.0f, up);
	
	while (!glfwWindowShouldClose(window)) 
		{
			glm::mat4 Projection = glm::perspective(fov, (float)HEIGHT / WIDTH, 0.1f, 300.0f)*Transform::translate(0, ProjectionY, 0);
			glm::mat4 View = cam.GetView();
			glm::mat4 MVP = Projection;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		   
			
			//sky
			glm::mat4 SkyMod;
			
			SkyMod = glm::mat4(glm::mat3(glm::rotate(SkyMod, -cam.GetYaw(), up)))*glm::rotate(SkyMod, -cam.GetPitch(), glm::cross(cam.GetCamFront(), cam.GetCamUp()));
			SkyMod = glm::rotate(SkyMod, 180.0f, up);
			Sky.drawModel(cam.GetCamPos(), SkyMod, MVP);
			//
			GroundMod = cam.UpdateCamera(window, actor)*Transform::translate(-actor.x, actor.y - 5, -actor.z)*Transform::scale(50, 50, 50);
			Ground.drawModel(cam.GetCamPos(), GroundMod, MVP);
			
			CharMod = cam.UpdateCamera(window, actor)*Transform::translate(actor.x, actor.y + 5, actor.z);//*Transform::scale(5, 5, 5);
			CharMod = glm::rotate(CharMod, cam.GetYaw(), cam.GetCamUp());
			Character.SetViewMatrix(View);
			Character.drawModel(cam.GetCamPos(), CharMod, MVP);
							
			moveActor(window);
			Character.UpdateLightPosition(lpos);//hack fix later
			Ground.UpdateLightPosition(lpos);
			Sky.UpdateLightPosition(lpos);
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
			fov += 1;
		if (key == GLFW_KEY_KP_SUBTRACT)
			fov -= 1;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
		if (SHCamera::radius >= 2.0f && SHCamera::radius <= 200.0f)
			SHCamera::radius += yoffset;
		else if (SHCamera::radius >=2.0f)
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
	
	actor.y = -20;
}