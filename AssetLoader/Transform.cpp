#include "Transform.h"

mat3 Transform::rotate(const float degrees, const vec3& axis1) {
	vec3 axis = glm::normalize(axis1);
	mat3 R;
	mat3 I(1.0f);
	mat3 second(
		axis.x*axis.x, axis.x*axis.y, axis.x*axis.z,
		axis.x*axis.y, axis.y*axis.y, axis.y*axis.z,
		axis.x*axis.z, axis.y*axis.z, axis.z*axis.z);
	glm::transpose(second);
	mat3 third(
		0, -axis.z, axis.y,
		axis.z, 0, -axis.x,
		-axis.y, axis.x, 0);
	glm::transpose(third);
	R = cos(glm::radians(degrees))*I + (1 - cos(glm::radians(degrees)))*second + sin(glm::radians(degrees))*third;
	return R;
}
void Transform::left(float degrees, vec3& eye, vec3& up) {
	up = glm::normalize(up);
	eye = eye*rotate(degrees, up);
}
void Transform::up(float degrees, vec3& eye, vec3& up) {
	vec3 a = glm::normalize(eye);
	up = glm::normalize(up);
	vec3 cross = glm::cross(a, up);

	eye = eye*rotate(degrees, cross);
	up = up*rotate(degrees, cross);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
	vec3 a = eye - center;

	vec3 w = glm::normalize(a);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::normalize(glm::cross(w, u));

	mat4 look(
		u.x, u.y, u.z, -glm::dot(u, eye),
		v.x, v.y, v.z, -glm::dot(u, eye),
		w.x, w.y, w.z, -glm::dot(w, eye),
		0, 0, 0, 1
		);

	look = glm::transpose(look);

	return look;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar) {
	float d = cos(glm::radians(fovy) / 2) / sin(glm::radians(fovy) / 2);
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = -(2 * zFar*zNear) / (zFar - zNear);
	
	 mat4 pers(
    d/aspect, 0, 0, 0,
	   0, d, 0, 0,
	   0, 0, A, B,
	   0, 0,-1, 0);
	 pers = glm::transpose(pers);
	 return pers;
	//return glm::perspective(fovy, aspect, zNear, zFar);

}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
	mat4 ret(
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1);
	ret = glm::transpose(ret);

	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
	mat4 ret(
		1, 0, 0, tx,
		0, 1, 0, ty,
		0, 0, 1, tz,
		0, 0, 0, 1);
	ret = glm::transpose(ret);
	return ret;
}
vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 ret = glm::normalize(y);
	return ret;
}
Transform::Transform(){}
Transform::~Transform(){}