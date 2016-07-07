#ifndef STATICVARIABLES_H
#define STATICVARIABLES_H
GLFWwindow* Control::Window;
Actor* Control::Pawn;
SHCamera* Control::Camera;
GLfloat Control::DeltaTime;
GLfloat SHCamera::radius;
glm::mat4 SHCamera::Projection;
glm::mat4 SHCamera::View;
#endif