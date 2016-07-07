#ifndef TRANSFORM_H
#define TRANSFORM_H

// Rotate is a helper function

// Include the helper glm library, including matrix transform extensions
#include<GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// glm provides vector, matrix classes like glsl
// Typedefs to make code more readable 

using mat3 = glm::mat3;
using mat4 = glm::mat4; 
using vec3 = glm::vec3; 
using vec4 = glm::vec4; 
const float pi = 3.14159265 ; // For portability across platforms

class Transform  
{
public:
	Transform();
	virtual ~Transform();
	template<typename T>static T Clamp(T x, T Min, T Max) {
		if (x < Min)
			x = Min;
		else if (x > Max)
			x = Max;

		return x;
	}
	template<typename T>static T Min(T A, T B) {
		if (A < B)
			return A;
		if (A > B)
			return B;
		return A;
	}
	template<typename T>static T Swap(T & a, T & b) {
		T temp = a;
		a = b;
		b = temp;
	}
	
	static GLfloat Smoothstep(GLfloat edge0, GLfloat edge1, GLfloat x);
	static GLfloat Smootherstep(GLfloat edge0, GLfloat edge1, GLfloat x);
	static GLfloat Lerp(GLfloat Start, GLfloat End, GLfloat Percent);
	static GLdouble BilinearInterpolate(GLuint HighX, GLuint LowX, GLuint HighZ, GLuint LowZ, GLdouble ExactX, GLdouble ExactZ, GLdouble P1, GLdouble P2, GLdouble P3, GLdouble P4);
	static GLdouble Interpolate(GLuint LowEdge, GLuint HighEdge, GLdouble Tweener, GLdouble FuncVal1, GLdouble FuncVal2);
	static glm::vec3 VecLerp(glm::vec3 Start, glm::vec3 End, GLfloat Percent);
	
	static void left(float degrees, vec3& eye, vec3& up);
	static void up(float degrees, vec3& eye, vec3& up);
	static mat4 lookAt(const vec3& eye, const vec3 &center, const vec3& up);
	static mat4 perspective(float fovy, float aspect, float zNear, float zFar);
    static mat3 RotateMat3(const float degrees, const vec3& axis) ;
	static mat4 RotateMat4(const float degrees, const vec3& axis);
    static mat4 scale(const float &sx, const float &sy, const float &sz) ; 
    static mat4 translate(const float &tx, const float &ty, const float &tz);
    static vec3 upvector(const vec3 &up, const vec3 &zvec) ;
};
#endif // !TRANSFORM_H
