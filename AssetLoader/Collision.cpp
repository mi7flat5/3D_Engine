#include "Collision.hpp"
#include"Transform.h"

Collision::Collision() { }
Collision::~Collision() {}
glm::vec3 Collision::Support(Collider a, Collider b, glm::vec3 Direction)const {

	glm::vec3 Point1 = a.GetFurthestPoint(Direction);
	glm::vec3 Point2 = b.GetFurthestPoint(-Direction);
	Point1 = Point1 - Point2;
	return  Point1;
}
bool Collision::IsCollision(Collider &First, Collider &Second) {

	int LoopMax = Transform::Min(First.PointsInCollider(), Second.PointsInCollider());

	glm::vec3 Direction = glm::vec3(0, 0, 1);
	glm::vec3 S = Support(First, Second, Direction);
	 
	Simplex.push_front(S);
	Direction = -S;

	for (int i =0;i<LoopMax;++i)
	{
		MostRecentAdded = Support(First, Second, Direction);
		if (glm::dot(MostRecentAdded, Direction) < 0)
		{
			Simplex.clear();
			return false;
		}
		else
		{
			Simplex.push_front(MostRecentAdded);
			if (BuildSimplex(Direction))
			{
				CollisionPoints = Simplex;
				Simplex.clear();
				return true;
			}
		}
	}
	Simplex.clear();
	return false;
}
bool Collision::BuildSimplex(glm::vec3& Direction)
{
	if (Simplex.size() == 2)
	{
		return Line(Direction);
	}
	if (Simplex.size()==3)
	{
		return Triangle(Direction);
	}
	if (Simplex.size() == 4)
	{
		return Tetrahedron(Direction);
	}
	return false;
}
bool Collision::Line(glm::vec3& Direction)
{
	glm::vec3 AB = Simplex[1]-Simplex[0];
	glm::vec3 AOrigin = -Simplex[0];
	if (glm::dot(AB, AOrigin) > 0)
	{
		Direction = glm::cross(glm::cross(AB, AOrigin), AB);
	}
	else {
		Direction = AOrigin;
		Simplex.pop_back();
	}
	return false;
}
bool Collision::Triangle(glm::vec3& Direction)
{
	glm::vec3 AOrigin = -Simplex[0];
	glm::vec3 AB = Simplex[1] - Simplex[0];
	glm::vec3 AC = Simplex[2] - Simplex[0];
	glm::vec3 ABC = glm::cross(AB, AC);
	glm::vec3 ABC_AC = glm::cross(ABC, AC);
	glm::vec3 AB_ABC = glm::cross(AB, ABC);
	
	if (glm::dot(ABC_AC, AOrigin) > 0)
	{
		if (glm::dot(AC, AOrigin) > 0)
		{
			Simplex[1] = Simplex[0];
			Simplex.pop_front();
			Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
		}
		else if (glm::dot(AB, AOrigin) > 0) {
			Simplex.pop_back();
			Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
		}
		else
		{
			Simplex.pop_back();
			Simplex.pop_back();
			Direction = AOrigin;
		}
		return false;
	}
	if (glm::dot(AB_ABC, AOrigin) > 0)
	{
		if (glm::dot(AB, AOrigin) > 0) {
			Simplex.pop_back();
			Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
		}
		if (glm::dot(AC, AOrigin) > 0)
		{
			Simplex[1] = Simplex[0];
			Simplex.pop_front();
			Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
		}
		else
		{
			Simplex.pop_back();
			Simplex.pop_back();
			Direction = AOrigin;
		}
		return false;
	}
	if (glm::dot(ABC, AOrigin) > 0)
	{
		Direction = ABC;
	}
	else
	{
		glm::vec3 hold = Simplex[1];
		Simplex[1] = Simplex[2];
		Simplex[2] = hold;
		Direction = -ABC;
	}
	
	return false;
}

bool Collision::Tetrahedron(glm::vec3& Direction)
{
	glm::vec3 AOrigin = -Simplex[0];
	glm::vec3 AB = Simplex[1] - Simplex[0];
	glm::vec3 AC = Simplex[2] - Simplex[0];
	glm::vec3 AD = Simplex[3] - Simplex[0];
	glm::vec3 ABC = glm::cross(AB, AC);
	glm::vec3 ACD = glm::cross(AC, AD);
	glm::vec3 ADB = glm::cross(AD, AB);
	const int OVER_ABC = 0x1;
	const int OVER_ACD = 0x2;
	const int OVER_ADB = 0x4;
	int PLANE_TEST = (glm::dot(ABC, AOrigin) > 0 ? OVER_ABC : 0) 
					|(glm::dot(ACD, AOrigin) > 0 ? OVER_ACD : 0)
					|(glm::dot(ADB, AOrigin) > 0 ? OVER_ADB : 0);

	switch (PLANE_TEST)
	{
		case (0):
			return true;
		case (OVER_ABC):
			if (glm::dot(glm::cross(ABC, AC), AOrigin) > 0)
			{
				Simplex[1] = Simplex[0];
				Simplex.pop_front();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AC, AOrigin),AOrigin);
				return false;
			}
			if (glm::dot(glm::cross(AB, ABC), AOrigin) > 0)
			{			
				Simplex.pop_back();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AB, AOrigin),AOrigin);
				return false;
			}
			Simplex.pop_back();
			Direction = ABC;
			break;
		case (OVER_ACD):
			Simplex[1] = Simplex[0];
			if (glm::dot(glm::cross(ACD, AC), AOrigin) > 0)
			{
				Simplex.pop_front();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
				return false;
			}
			if (glm::dot(glm::cross(AD, ACD), AOrigin) > 0)
			{
				Simplex[2] = Simplex[3];
				Simplex.pop_back();
				Simplex.pop_front();
				Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_front();
			Direction = ACD;
			break;
		case (OVER_ADB):
			Simplex[2] = Simplex[1];
			Simplex[1] = Simplex[3];
			if (glm::dot(glm::cross(ADB, AB), AOrigin) > 0)
			{
				Simplex[1] = Simplex[0];
				Simplex.pop_front();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
				return false;
			}
			if (glm::dot(glm::cross(AD, ADB), AOrigin) > 0)
			{
				Simplex.pop_back();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_back();
			Direction = ADB;
			break;
		case(OVER_ABC|OVER_ACD):
		
			if (glm::dot(glm::cross(ABC, AC), AOrigin) > 0)
			{
				Simplex[1] = Simplex[0];
				if (glm::dot(glm::cross(ACD, AC), AOrigin) > 0)
				{
					Simplex.pop_front();
					Simplex.pop_back();
					Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
					return false;
				}
				if (glm::dot(glm::cross(AD, ACD), AOrigin) > 0)
				{
					Simplex[2] = Simplex[3];
					Simplex.pop_back();
					Simplex.pop_front();
					Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
					return false;
				}
				Simplex.pop_front();
				Direction = ACD;
				break;
			}
			if (glm::dot(glm::cross(AB, ABC), AOrigin) > 0)
			{
				Simplex.pop_back();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_back();
			Direction = ABC;
			break;
		case(OVER_ABC|OVER_ADB):
			if (glm::dot(glm::cross(ABC, AC), AOrigin) > 0)
			{
				Simplex[2] = Simplex[1];
				Simplex[1] = Simplex[3];
				if (glm::dot(glm::cross(ADB, AB), AOrigin) > 0)
				{
					Simplex[1] = Simplex[0];
					Simplex.pop_front();
					Simplex.pop_back();
					Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
					return false;
				}
				if (glm::dot(glm::cross(AD, ADB), AOrigin) > 0)
				{
					Simplex.pop_back();
					Simplex.pop_back();
					Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
					return false;
				}
				Simplex.pop_back();
				Direction = ADB;
				break;
			}
			if (glm::dot(glm::cross(AB, ABC), AOrigin) > 0)
			{
				Simplex.pop_back();
				Simplex.pop_back();
				Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_back();
			Direction = ABC;
			break;
		case(OVER_ACD|OVER_ADB):
			if (glm::dot(glm::cross(ACD, AC), AOrigin) > 0)
			{
				Simplex[2] = Simplex[1];
				Simplex[1] = Simplex[3];
				if (glm::dot(glm::cross(ADB, AB), AOrigin) > 0)
				{
					Simplex[1] = Simplex[0];
					Simplex.pop_front();
					Simplex.pop_back();
					Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
					return false;
				}
				if (glm::dot(glm::cross(AD, ADB), AOrigin) > 0)
				{
					Simplex.pop_back();
					Simplex.pop_back();
					Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
					return false;
				}
				Simplex.pop_back();
				Direction = ADB;
				break;
			}
			if (glm::dot(glm::cross(AD, ACD), AOrigin) > 0)
			{
				Simplex[2] = Simplex[3];
				Simplex.pop_back();
				Simplex.pop_front();
				Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
				return false;
			}
			Simplex[1] = Simplex[0];
			Simplex.pop_front();
			Direction = ACD;
			break;
		default:
			return false;
			break;
	}
	return false;
}
glm::vec3 Collision::Dcross(const glm::vec3 &a, const  glm::vec3 &b) {
	return glm::cross(glm::cross(a,b),b);
}

