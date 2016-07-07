#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include<vector>
#include<string>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<GL/glew.h>
#include"Shaders.hpp"
class Collider
{
	std::vector<glm::vec3> Verts, RenderVerts;
	std::vector<GLuint> Indices;
	glm::mat4 ColliderModel;
	GLuint  VAO, VBO,EBO, ProjectionMatrixID, ViewMatrixID, ModelMatrixID;
	Shaders * ModelShader;
	
public:
	int PointsInCollider();
	void SetModelMatrix(glm::mat4);
	Collider(const std::string& );
	virtual ~Collider();
	void Draw() ;
	glm::vec3 GetFurthestPoint(const glm::vec3 & DirectionVector) const;
	
};

#endif // !COLLIDER_HPP