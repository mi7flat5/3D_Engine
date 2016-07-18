#ifndef COLLISION_HPP
#define COLLISION_HPP

#include"Collider.hpp"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<deque>

struct Triangle {

	glm::vec3 Normal;
	glm::vec3 Points[3];
	
	inline Triangle(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
		
		Points[0] = a;
		Points[1] = b;
		Points[2] = c;
		Normal = glm::normalize(glm::cross(b - a, c - a));
	}
	
};

struct Edge {
	glm::vec3 Edg[3];
	glm::vec3 uniqueId;
	
	inline Edge(const glm::vec3 &a, const glm::vec3 &b) {
	
		Edg[0] = a;
		Edg[1] = b;
		uniqueId = b - a;
	}

};
class Collision
{
private:
	std::deque<glm::vec3> Simplex;
	std::deque<glm::vec3> ObjectPointsA;
	Collider* ColliderA;
	Collider* ColliderB;
	glm::vec3 MostRecentAdded;
	glm::vec3 BarycentricCoefficents, ContactPointA,ContactPointNormal;
	
	GLfloat PenetrationDepth;
	inline bool BuildSimplex(glm::vec3 &Direction);
	inline bool FindEdge(glm::vec3 & dir);
	inline bool FindTriangle(glm::vec3 & Direction);
	inline bool FindTetrahedron(glm::vec3 & Direction);
		
	glm::vec3 Dcross(const glm::vec3 & a, const glm::vec3 & b);
		
	inline bool ExpandPolytope();
	inline void UpdateCollisionInfo(const Triangle * triangle);
	inline glm::vec3 EPASupport(Collider * a, Collider * b, glm::vec3 & Direction);

	inline glm::vec3 Support(Collider & a, Collider & b, glm::vec3 & Direction);
		
public:
	Collision();
	~Collision();
	glm::vec3 GetCollisionNormal();

	GLfloat GetPenetrationDepth();

	bool Tetrahedron(glm::vec3 & Direction);
	
	bool Collision::IsCollision( Collider & First,  Collider & Second);
};
#endif // !COLLISION_HPP
