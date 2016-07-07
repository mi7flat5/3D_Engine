#ifndef COLLISION_HPP
#define COLLISION_HPP

#include"Collider.hpp"
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<deque>
class Collision
{
private:
	std::deque<glm::vec3> Simplex;
	std::deque<glm::vec3> CollisionPoints;
	glm::vec3 MostRecentAdded;

	bool BuildSimplex(glm::vec3 &Direction);
	bool Line(glm::vec3 & dir);
	bool Triangle(glm::vec3 & Direction);
	bool Tetrahedron(glm::vec3 & Direction);

	glm::vec3 Support(Collider a, Collider b, glm::vec3 Direction) const;
	glm::vec3 Dcross(const glm::vec3 & a, const glm::vec3 & b);
	
public:
	Collision();
	~Collision();
	bool Collision::IsCollision(Collider & First, Collider & Second);
};
#endif // !COLLISION_HPP
