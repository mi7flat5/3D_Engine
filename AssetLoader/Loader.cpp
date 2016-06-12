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
glm::vec3 center(0, 0, 0);
glm::vec3 up(0, 1, 0);

glm::vec3 actor(0, 0, 0);

glm::vec3 lpos(0, 200,  200);
glm::vec3 lightColor(1, 1, 1);
float actorY = 0;
SHCamera cam;
GLfloat SHCamera::radius;
glm::mat4 Model::View;
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
	
	Model Character("assets/GrossFace.fbx", MaterialType::TEXTURE_2D_REFLECT);
	Model Ground("assets/ground_d.fbx", MaterialType::TEXTURE_2D_DISPLACEMENT);
	Model Sky("assets/box.fbx", MaterialType::TEXTURE_3D);
	//Model UnseenSky("assets/box.fbx", MaterialType::TEXTURE_3D_REFLECTOR);
		//*Transform::scale(.02,.02,.02)
	
	glm::mat4 GroundMod(1.0f);
	glm::mat4 CharMod(1.0f);
	glm::mat4 SkyMod = cam.GetView();
	
	while (!glfwWindowShouldClose(window)) 
		{
			glm::mat4 Projection = glm::perspective(fov, (float)HEIGHT / WIDTH, 0.1f, 300.0f);
		
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Model::View = cam.UpdateCamera(window, actor);
			
			//sky
			glm::mat4 vew = cam.GetView();
			
			Sky.drawModel(cam.GetCamPos(), SkyMod, Projection,vew);

			//glm::mat4 SkyReflector(1.0f);
			//SkyReflector = glm::rotate(SkyMod, 180.0f, up);*/
			//UnseenSky.drawModel(cam.GetCamPos(), SkyReflector, Projection);
			if (glfwGetKey(window, GLFW_KEY_F1))
				Ground.SetMeshMode(3);
			if (glfwGetKey(window, GLFW_KEY_F2))
				Ground.SetMeshMode(7);
		
			GroundMod = Transform::translate(0, -5, 0);
			
			Ground.drawModel(cam.GetCamPos(), GroundMod*Transform::scale(150, 1,150), Projection,Model::View);
		
			//
			
			//CharMod = glm::rotate(CharMod, cam.GetYaw(), cam.GetCamUp());
			
			CharMod = Transform::translate(actor.x,actor.y,actor.z);
			//Character.UpdateLightPosition(glm::vec4(actor.x, actor.y + 100, actor.z, 1.0f));
			Character.drawModel(cam.GetCamPos(), glm::rotate(CharMod, cam.GetYaw(), cam.GetCamUp()), Projection, Model::View);
			Ground.UpdateLightPosition(glm::vec4(cam.GetCamFront(),1.0f));
			moveActor(window);
			
			Ground.UpdateLightPosition(glm::vec4(lpos,0.0f));
			//Sky.UpdateLightPosition(glm::vec4(lpos, 0.0f));
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
			ProjectionY += 1;
		if (key == GLFW_KEY_KP_SUBTRACT)
			ProjectionY -= 1;
		
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (SHCamera::radius >= 2.0f && SHCamera::radius <= 200.0f)
		SHCamera::radius += yoffset;
	else if (SHCamera::radius > 2.0f)
		SHCamera::radius -= yoffset;
	else if (SHCamera::radius < 2.0f)
		SHCamera::radius = 2.0f;
}
void moveActor(GLFWwindow*window)
{
	float rate = 0.5;
	if (glfwGetKey( window, GLFW_KEY_D))
	{
		actor += glm::normalize(glm::cross(cam.GetCamFront(), cam.GetCamUp())) *rate;
		glm::vec3 FrontVector = glm::normalize(glm::cross(cam.GetCamFront(), cam.GetCamUp()));
	
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
	
	//actor.y = 0;
}