#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include<vector>
#include<string>
#include"glm-0.9.2.7\glm\glm.hpp"
#include"glm-0.9.2.7\glm\gtc\matrix_transform.hpp"
#include"glm-0.9.2.7\glm\gtc\type_ptr.hpp"

#include"Shaders.hpp"

class Collider
{
	std::vector<glm::vec3> Verts, RenderVerts;
	std::vector<GLuint> Indices;
	glm::mat4 ColliderModel;
	GLuint  VAO, VBO,EBO, ProjectionMatrixID, 
		ViewMatrixID, ModelMatrixID,
		DebugPoint1, DebugPoint2;

	GLuint tVao, tVbo;

	Shaders * ModelShader;
	Shaders * DebuglineShader;
	GLuint SphereRadius;

	
	
	
public:
	int PointsInCollider();
	void SetModelMatrix(glm::mat4);
	Collider(const std::string& );
	Collider();
	virtual ~Collider();
	void Draw() ;
	glm::mat4 GetColliderMatrix()const;
	
	 glm::vec3 GetFurthestPoint(const glm::vec3 & DirectionVector);

	 bool SphereRayCollider(glm::vec4 RayStart, glm::vec4 RayDirection);

	 bool SphereRayCollider(glm::vec3 RayStart, glm::vec3 RayDirection);

	 glm::vec3 GetPosition() const;

	 bool SphereSphereCollider(Collider * TargetCollider);

	 GLuint GetRadius()const;
	 glm::mat4 GetColliderModelMatrix()const;
	
	
};

#endif // !COLLIDER_HPP