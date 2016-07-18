
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
GLuint WIDTH = 600;
GLuint HEIGHT = 800;
GLfloat fov = 90.0f;

int main() { 
	
	SHCamera cam(WIDTH, HEIGHT);
	GLFWwindow* window = initialize();
	if (!window)
		return -1;

	Collision tester;
	Collider test("assets/ColliderBox.obj");
	Collider test2("assets/box.fbx");
	
	
	CubeMap Sky(MeshType::SKYBOX);
	Terrain Ground("assets/ParisTerrain.fbx", MeshType::TERRAIN);
	Actor Character("assets/GrossFace.fbx", MeshType::TEXTURE_2D_REFLECT, &cam, &Ground, &test);
	Actor arrow("assets/sphere.fbx", MeshType::TEXTURE_2D_REFLECT, &cam, &Ground, &test2);
	
	
	arrow.SetTransforms(Transform::scale(20,20,20));
	arrow.SetPosition(glm::vec3(-30,15,-30));
	Character.SetPosition(glm::vec3(30,15,30));
		
	arrow.Draw();
	Character.Draw();
	Control control(window,&Character,&cam);
	
	SHCamera::Projection = glm::perspective(fov, (float)HEIGHT / WIDTH, 0.1f, 1800.0f);
	while (!glfwWindowShouldClose(window)) 
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();
			control.UpdateControls();

			
		
			if (tester.IsCollision(test, test2))
			{

				Character.CorrectCollision(tester.GetCollisionNormal(), tester.GetPenetrationDepth());
				//arrow.CorrectCollision(-tester.GetCollisionNormal(), tester.GetPenetrationDepth());
				VECCONSOLEOUT(Character.GetPosition(),"Char Position :")
			}
			
		
		

			static double timepassed;
			double seconds = glfwGetTime();
			if (seconds - timepassed > 0.009) {
				if (glfwGetKey(window, GLFW_KEY_L))
					control.SetPawn(&arrow);
				if (glfwGetKey(window, GLFW_KEY_K))
					control.SetPawn(&Character);
			}
			timepassed = glfwGetTime();
						
			Sky.Draw();
			Ground.Draw();
			Character.Draw();
			arrow.Draw();
			test.Draw();
			test2.Draw();
			
		
			
			
		
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
