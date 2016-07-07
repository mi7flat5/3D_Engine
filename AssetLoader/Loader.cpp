
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/glut.h>
#include"Model.h"
#include"Transform.h"
#include"SHCamera.h"
#include"Terrain.hpp"
#include"CubeMap.hpp"
#include"Actor.hpp"
#include"Collider.hpp"
#include"Collision.hpp"
#include"Control.hpp"

#include"StaticVariables.h"

GLFWwindow* initialize();
GLuint WIDTH = 1200;
GLuint HEIGHT = 1600;
GLfloat fov = 90.0f;

int main() { 
	
	SHCamera cam(WIDTH, HEIGHT);
	GLFWwindow* window = initialize();
	if (!window)
		return -1;
	Collider test("assets/CollideBox.fbx");
	Collider test2("assets/CollideBox.fbx");
	Collision tester;
	CubeMap Sky("assets/box.fbx", MeshType::SKYBOX);
	Terrain Ground("assets/ParisTerrain.fbx", MeshType::TERRAIN);
	Actor Character("assets/GrossFace.fbx", MeshType::TEXTURE_2D_REFLECT, &cam, &Ground, &test);
	Actor arrow("assets/arrow.fbx", MeshType::TEXTURE_2D_REFLECT, &cam, &Ground, &test2);
	arrow.SetPosition(glm::vec3(0,5,-15));
	arrow.SetTransforms(Transform::translate(0,5,0)*Transform::scale(10,10,10));
	
	
	Model Pave("assets/ground_d.fbx", MeshType::TEXTURE_2D_DISPLACEMENT);
	Model FTargetLoc("assets/arrow.fbx", MeshType::NO_TEXTURE);
	SHCamera::Projection = glm::perspective(fov, (float)HEIGHT / WIDTH, 0.1f, 1800.0f);
	Control control(window,&Character,&cam);
	cam.SetHome(arrow.GetPosition());
	while (!glfwWindowShouldClose(window)) 
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//Cubemap			
			Sky.Draw();
						
			//Pave.Draw(cam.GetCamPos(), Transform::translate(0, 13, 0)*Transform::scale(430, 100, 430));
			
			//Terrain
			Ground.Draw();
			
			//FTargetLoc.Draw(cam.GetCamPos(), 
			//	Transform::translate(Character.GetTargetPosition().x, Character.GetTargetPosition().y+5, Character.GetTargetPosition().z)
			//	*glm::rotate(glm::mat4(),cam.GetYaw(),cam.GetCamUp())/*Transform::scale(5.0,5.0,5.0)*/);
			test.Draw();
			test2.Draw();
			Character.Draw();
			arrow.Draw();
			static double timepassed;
			double seconds = glfwGetTime();
			if (seconds - timepassed > 0.009) {
				if (glfwGetKey(window, GLFW_KEY_L))
					control.SetPawn(&arrow);
				if (glfwGetKey(window, GLFW_KEY_K))
					control.SetPawn(&Character);
			}
			timepassed = glfwGetTime();
			
			if (tester.IsCollision(test, test2))
				std::cout << "\nCollision detected";
			control.UpdateControls();
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		
	glfwTerminate();
	
	return 0; 
}


GLFWwindow * initialize()
{
	glfwInit();
	
	GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Loader", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return nullptr;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
	

	glViewport(0, 0, HEIGHT, WIDTH);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	return window;
}
