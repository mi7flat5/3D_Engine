#include "SHCamera.h"

#include<iostream>
#include"Transform.h"
#include"Control.hpp"


SHCamera::SHCamera (GLuint Width,GLuint Height) : pitch(15), 
											      campos(glm::vec3(0, 5, 5)),
												  camfront(glm::vec3(0, 0, -1)),
												  camup(glm::vec3(0, 1, 0)),
												  yaw(0),
												  FieldOfView(90.0f)
{
	
	WIDTH = Width;
	HEIGHT = Height;
	Home = glm::vec3(0,16,0);
	radius = 20.0;
	TargSelect = false;
	Moving = false;
	UpdateOffsetsVectors();
	TerrainHeight = 50;
	Target = glm::vec3(0, 21, 0);



}
SHCamera::~SHCamera(){}


glm::vec3 SHCamera::GetHome()const {  return Home; }
void SHCamera::SwithTarget() {
	
	if (!Moving)
	{
		if (TargSelect)
			TargSelect = false;
		else  TargSelect = true;
		Moving = true;
	}
	
}
bool SHCamera::IsMoving() { return Moving; }
glm::mat4 SHCamera::UpdateCamera(glm::vec3 &actor)
{
	UpdateOffsetsVectors();
	
	if (!Moving)
		{
			if (!TargSelect)
				Target = actor;
			if (TargSelect)
				Target = Home;
		}
	if (Moving)
		{
			if (!TargSelect)
				Target = MovePosition(Home, actor);
			if (TargSelect)
			{
				Target = MovePositionReverse(Home,actor);
			}
		}
		
	//if (vdist < 0)
	//	vdist = 0;
	glm::vec3 NewCampos;
	NewCampos.y =vdist + Target.y;	//TODO add parameter for vertical distance offset
	if (NewCampos.y < TerrainHeight)
		{
			NewCampos.y =   TerrainHeight+2;

		}
	
	NewCampos.x = Target.x - Xoffset;
	NewCampos.z = Target.z - Zoffset;
	campos.x = NewCampos.x;
	campos.y = Transform::Lerp(campos.y, NewCampos.y, .30);
	campos.z = NewCampos.z;
	SHCamera::View = glm::lookAt(campos, campos + camfront, camup);
	
	return SHCamera::View;
}
void SHCamera::UpdateOffsetsVectors()
{
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89)
		pitch = -89;

	theta = 90 - yaw;
	phi = 90 - pitch;

	front.x = radius * sin(glm::radians(phi))*cos(glm::radians(theta));
	//if (campos.y > TerrainHeight)
		front.y = radius * cos(glm::radians(pitch + 90))+10; //TODO add parameter for adjusting y facing offset
	front.z = radius * sin(glm::radians(phi))*sin(glm::radians(theta));
	camfront = glm::normalize(front);
	
	vdist = radius*sin(glm::radians(pitch));
	hdist = radius*cos(glm::radians(pitch));
	Xoffset = hdist*sin(glm::radians(yaw));
	Zoffset = hdist*cos(glm::radians(yaw));

	Right = glm::normalize(glm::cross(camup,camfront));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
														
}
glm::vec3 SHCamera::GetXZfront() {
	
	
	return glm::cross(Right,camup);
}
glm::vec3 SHCamera::MovePosition(glm::vec3 Start, glm::vec3 end) {

	glm::vec3 Current;
	static GLfloat Time;

	if (Time <= 100.0)
	{
		Time += 20 * Control::DeltaTime;

		Current.z = Transform::Smoothstep(Start.z, end.z, Time);
		Current.x = Transform::Smootherstep(Start.x, end.x, Time);
		Current.y = Transform::Smoothstep(Start.y, end.y, Time);
	}
	if (Time >= 100)
	{
		Moving = false;
		Time = 0;
		return end;
	}
	return Current;
}
glm::vec3 SHCamera::MovePositionReverse(glm::vec3 Start, glm::vec3 end) {

	glm::vec3 Current;
	static GLfloat Time;
	static bool firstrun;
	if (!firstrun)
	{
		Time = 100;
		firstrun = true;

	}
	if (Time > 0.0)
	{
		Time -= 20 * Control::DeltaTime;

		Current.z = Transform::Smootherstep(Start.z, end.z, Time);
		Current.x = Transform::Smoothstep(Start.x, end.x, Time);
		Current.y = Transform::Smootherstep(Start.y, end.y, Time);
	}
	if (Time <= 0)
	{
		Moving = false;
		Time = 100;
		return Start;
	}
	return Current;
}
void SHCamera::SetYaw(GLfloat InYaw) {
	yaw += InYaw;
}
void SHCamera::SetPitch(GLfloat InPitch)
{
	pitch += InPitch;
}
glm::vec3 SHCamera::GetRight()
{
	return Right;
}
glm::mat4 SHCamera::UpdateCamera() {
	
	
	return glm::lookAt(glm::vec3(campos), glm::vec3(campos) + camfront, camup); 
}

glm::vec3 SHCamera::GetCamPos()const
{
	return campos;
}

glm::vec3 SHCamera::GetCamFront()const
{
	return camfront;
}

glm::vec3 SHCamera::GetCamUp()const
{
	return camup;
}

GLfloat SHCamera::GetYaw() const
{
	return yaw;
}
void SHCamera::SetHome(glm::vec3  &InHome)
{
	Home = InHome;
}

glm::mat4 SHCamera::GetView() const
{
	return glm::mat4(glm::mat3(CurrentView));
}
GLfloat SHCamera::GetPitch() const
{
	return pitch;
}
void SHCamera::SetCamUp(glm::vec3 InUp)
{
	camup = InUp;
}
void SHCamera::SetTerrainHeight(GLuint InHeight)
{
	TerrainHeight = InHeight;
}


