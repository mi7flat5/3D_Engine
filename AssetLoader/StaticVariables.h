#ifndef STATICVARIABLES_H
#define STATICVARIABLES_H
#include"include\GL\glew.h"
#include"include/GLFW/glfw3.h"
#include"include\GL\glut.h"


#include"Model.h"
#include"Transform.h"
#include"SHCamera.h"
#include"Terrain.hpp"
#include"CubeMap.hpp"
#include"Actor.hpp"
#include"Collider.hpp"
#include"Collision.hpp"
#include"Control.hpp"
#define VECCONSOLEOUT(X,STRING) std::cout<<'\n'<<STRING<<X.x<<", "<<X.y<<", "<<X.z;
GLFWwindow* Control::Window;
Actor* Control::Pawn;
SHCamera* Control::Camera;
GLfloat Control::DeltaTime;
GLfloat SHCamera::radius;
glm::mat4 SHCamera::Projection;
glm::mat4 SHCamera::View;

GLuint WIDTH = 600;
GLuint HEIGHT = 800;
#endif