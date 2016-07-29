#include"StaticVariables.h"
#include<thread>
#include <future>
#include"GraphicsDebug.h"
#include<functional>
#include"Observer.hpp"
GLFWwindow* initialize();
GLfloat fov = 90.0f;


int main() { 
	
	
	GLFWwindow* window = initialize();
	if (!window)
		return -1;
	SHCamera cam(WIDTH, HEIGHT);

	Collision tester;
	Collider test("assets/ColliderBox.obj");
	Collider test2("assets/ColliderBox.obj");
	Collider nocollide("assets/ColliderBox.obj");

	std::unique_ptr<CubeMap> Sky = std::make_unique<CubeMap>( MeshType::SKYBOX);
	std::unique_ptr<Terrain> Ground = std::make_unique<Terrain>("assets/ParisTerrain.fbx", MeshType::TERRAIN);
	Actor* Character = new Actor("assets/box.fbx", MeshType::TEXTURE_2D_REFLECT, &cam, &*Ground, &test);
	
	Actor Sphere("assets/sphere.fbx", MeshType::TEXTURE_2D_REFLECT, &cam, &*Ground, &test2);
	Actor Arrow("assets/arrow.fbx", MeshType::NO_TEXTURE, &cam, &*Ground, &nocollide);

	
	
	Sphere.SetTransforms(Transform::scale(6,6,6));
	Character->SetTransforms(Transform::scale(3, 3, 3));
	Arrow.SetTransforms(Transform::scale(3, 3, 3));
	Character->SetPosition(glm::vec3(30, 15, 300));
	Sphere.SetPosition(glm::vec3(-30,15,-300));
	
		
	
	

	Sphere.Draw();
	Character->Draw();
	Arrow.Draw();
	Control control(window,&*Character,&cam);
	control.SetTarget(&Arrow);
	SHCamera::Projection = glm::perspective(fov, (float)HEIGHT / WIDTH, 0.1f, 1800.0f);
	
	while (!glfwWindowShouldClose(window)) 
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();
			control.UpdateControls();

			
		
				if (glfwGetKey(window, GLFW_KEY_L))
					control.SetPawn(&Arrow);
				if (glfwGetKey(window, GLFW_KEY_K))
					control.SetPawn(&*Character);
		
						
			auto f = std::async(&Collision::IsCollision,&tester, test,test2);
				
			if (f.get())
			{
				
				std::thread t2(&Actor::CorrectCollision, Character, tester.GetCollisionNormal(), tester.GetPenetrationDepth()), 
					t3(&Actor::CorrectCollision, &Sphere, -tester.GetCollisionNormal()*10.0f, tester.GetPenetrationDepth());
				VECCONSOLEOUT(Character->GetPosition(), "Char Position :")
					t2.join();
					t3.join();
			}
					
			Character->CheckSphereCollision(Arrow.GetCollider());

			Sky->Draw();
			Ground->Draw();
			Character->Draw();
			Sphere.Draw();
			Arrow.Draw();

			
		
					
			glfwSwapBuffers(window);
					
		}
	
	glfwTerminate();
	delete Character;
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
