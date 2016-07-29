#include "Collision.hpp"
#include"Transform.h"
#include<iostream>
#include<list>
#include <numeric>
#define VECCONSOLEOUT(VEC,STRING) std::cout<<'\n'<<STRING<<VEC.x<<", "<<VEC.y<<", "<<VEC.z;
#define SQR(x) x*x;
Collision::Collision() { }
Collision::~Collision() {}


bool Collision::ExpandPolytope() {
	

	const float _EXIT_GROWTH_THRESHOLD = 0.001f;
	const unsigned _EXIT_ITERATION_LIMIT = 20;
	unsigned _EXIT_ITERATION_CUR = 0;

	std::list<Triangle> lst_triangles;
	std::list<Edge> lst_edges;


	auto lam_addEdge = [&](const glm::vec3 &a, const glm::vec3 &b)->void {
		for (auto it = lst_edges.begin(); it != lst_edges.end(); it++) {
			if (it->uniqueId == b - a || it->uniqueId == a - b  ) {
				//opposite edge found, remove it and do not add new one
				lst_edges.erase(it);
				return;
			}
		}
		lst_edges.emplace_back(a, b);
	};

	// add the GJK simplex triangles to the list
	lst_triangles.emplace_back(Triangle(Simplex[0], Simplex[1], Simplex[2]));
	lst_triangles.emplace_back(Triangle(Simplex[0], Simplex[2], Simplex[3]));
	lst_triangles.emplace_back(Triangle(Simplex[0], Simplex[3], Simplex[1]));
	lst_triangles.emplace_back(Triangle(Simplex[1], Simplex[3], Simplex[2]));
	

	while (true) {
		if (_EXIT_ITERATION_CUR++ >= _EXIT_ITERATION_LIMIT) return false;

		// find closest triangle to origin
		std::list<Triangle>::iterator closestTriangle_it;
		float closestTriangle_dst = FLT_MAX;
		for (auto it = lst_triangles.begin(); it != lst_triangles.end(); it++) {
			const float dst = (glm::dot(it->Normal , it->Points[0]));
			if (dst < closestTriangle_dst) {
				closestTriangle_dst = dst;
				closestTriangle_it = it;
			}
		}

		// get the next support point in front of the current triangle, away from the origin
		glm::vec3 entry_new_support = EPASupport( ColliderA, ColliderB ,closestTriangle_it->Normal);

		// checks how much further this new point will take us from the origin
		// if it is not far enough then we assume we have found the closest triangle
		// on the hull from the origin
		const float newDst = glm::dot(closestTriangle_it->Normal , entry_new_support);
		const float growth = newDst - closestTriangle_dst;
		//if(thisItGrowth < 0) break;
		if ((growth < _EXIT_GROWTH_THRESHOLD)) {
					
			UpdateCollisionInfo(&*closestTriangle_it);
				//return extrapolateContactInformation(&*closestTriangle_it);
				return true;
			break;
		}

		for (auto it = lst_triangles.begin(); it != lst_triangles.end();) {
			// can this face be 'seen' by entry_new_support?
			if (glm::dot(it->Normal , (entry_new_support - it->Points[0])) > 0) {
				lam_addEdge(it->Points[0], it->Points[1]);
				lam_addEdge(it->Points[1], it->Points[2]);
				lam_addEdge(it->Points[2], it->Points[0]);
				it = lst_triangles.erase(it);
				continue;
			}
			it++;
		}

		// create new triangles from the edges in the edge list
		for (auto it = lst_edges.begin(); it != lst_edges.end(); it++) {
			lst_triangles.emplace_back(entry_new_support, it->Edg[0], it->Edg[1]);
		}

		lst_edges.clear();
	}

	return false;
}

bool Collision::IsCollision(Collider &First, Collider &Second) {

	int LoopMax = Transform::Min(First.PointsInCollider(), Second.PointsInCollider());
	ColliderA = &First;
	ColliderB = &Second;
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
			ObjectPointsA.clear();
			
			return false;
			
		}
		else
		{
			Simplex.push_front(MostRecentAdded);
			if (BuildSimplex(Direction))
			{
				
				VECCONSOLEOUT(ContactPointA, "Contact Point A: ")
					VECCONSOLEOUT(ContactPointNormal, "Contact Point Normal: ")
					std::cout<<"\nPenetration Depth: " << PenetrationDepth;
						
				ObjectPointsA.clear();
			
				
				Simplex.clear();
				return true;
			}
		}
	}
	Simplex.clear();
	ObjectPointsA.clear();
	
	return false;
}
bool Collision::BuildSimplex(glm::vec3& Direction)
{
	if (Simplex.size() == 2)
	{
		return FindEdge(Direction);
	}
	if (Simplex.size()==3)
	{
		return FindTriangle(Direction);
	}
	if (Simplex.size() == 4)
	{
		return FindTetrahedron(Direction);
	}
	return false;
}
bool Collision::FindEdge(glm::vec3& Direction)
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
		ObjectPointsA.pop_back();
		
	}
	return false;
}
bool Collision::FindTriangle(glm::vec3& Direction)
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
			ObjectPointsA[1] = ObjectPointsA[0];
			Simplex.pop_front();
			ObjectPointsA.pop_front();
			Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
		}
		else if (glm::dot(AB, AOrigin) > 0) {
			Simplex.pop_back();
			ObjectPointsA.pop_back();
			Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
		}
		else
		{
			Simplex.pop_back();
			Simplex.pop_back();
			ObjectPointsA.pop_back();
			ObjectPointsA.pop_back();
			Direction = AOrigin;
		}
		return false;
	}
	if (glm::dot(AB_ABC, AOrigin) > 0)
	{
		if (glm::dot(AB, AOrigin) > 0) {
			Simplex.pop_back();
			ObjectPointsA.pop_back();
			Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
		}
		if (glm::dot(AC, AOrigin) > 0)
		{
			Simplex[1] = Simplex[0];
			Simplex.pop_front();
		
			
			ObjectPointsA[1] = ObjectPointsA[0];
			ObjectPointsA.pop_front();
		
			Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
		}
		else
		{
			Simplex.pop_back();
			Simplex.pop_back();
			ObjectPointsA.pop_back();
			ObjectPointsA.pop_back();
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

		hold = ObjectPointsA[1];
		ObjectPointsA[1] = ObjectPointsA[2];
		ObjectPointsA[2] = hold;

		Direction = -ABC;
	}

	return false;
}

bool Collision::FindTetrahedron(glm::vec3& Direction)
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
	const int INSIDE_TETRAHEDRON = 0x0;
	
	int PLANE_TEST = (glm::dot(ABC, AOrigin) > 0 ? OVER_ABC : INSIDE_TETRAHEDRON)
		| (glm::dot(ACD, AOrigin) > 0 ? OVER_ACD : INSIDE_TETRAHEDRON)
		| (glm::dot(ADB, AOrigin) > 0 ? OVER_ADB : INSIDE_TETRAHEDRON);

	switch (PLANE_TEST)
	{
		case (INSIDE_TETRAHEDRON):
			
			ExpandPolytope();
			return true;
		case (OVER_ABC):
			if (glm::dot(glm::cross(ABC, AC), AOrigin) > 0)
			{
				Simplex[1] = Simplex[0];
				ObjectPointsA[1] = ObjectPointsA[0];
			
				Simplex.pop_front();
				Simplex.pop_back();
				ObjectPointsA.pop_front();
				ObjectPointsA.pop_back();
			
			
				Direction = glm::cross(glm::cross(AC, AOrigin),AOrigin);
				return false;
			}
			if (glm::dot(glm::cross(AB, ABC), AOrigin) > 0)
			{			
				Simplex.pop_back();
				Simplex.pop_back();
				ObjectPointsA.pop_back();
				ObjectPointsA.pop_back();
				
				Direction = glm::cross(glm::cross(AB, AOrigin),AOrigin);
				return false;
			}
			Simplex.pop_back();
			ObjectPointsA.pop_back();
		
			Direction = ABC;
			return false;
		case (OVER_ACD):
			Simplex[1] = Simplex[0];
			ObjectPointsA[1] = ObjectPointsA[0];
		
			if (glm::dot(glm::cross(ACD, AC), AOrigin) > 0)
			{
				Simplex.pop_front();
				Simplex.pop_back();
				ObjectPointsA.pop_front();
				ObjectPointsA.pop_back();
			
				Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
				return false;
			}
			if (glm::dot(glm::cross(AD, ACD), AOrigin) > 0)
			{
				Simplex[2] = Simplex[3];
				Simplex.pop_back();
				Simplex.pop_front();
				ObjectPointsA[2] = ObjectPointsA[3];
				ObjectPointsA.pop_back();
				ObjectPointsA.pop_front();
			
			
				Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_front();
			ObjectPointsA.pop_front();
		
			Direction = ACD;
			return false;
		case (OVER_ADB):
			Simplex[2] = Simplex[1];
			Simplex[1] = Simplex[3];
			ObjectPointsA[2] = ObjectPointsA[1];
			ObjectPointsA[1] = ObjectPointsA[3];
		
			
			if (glm::dot(glm::cross(ADB, AB), AOrigin) > 0)
			{
				Simplex[1] = Simplex[0];
				Simplex.pop_front();
				Simplex.pop_back();
				ObjectPointsA[1] = ObjectPointsA[0];
				ObjectPointsA.pop_front();
				ObjectPointsA.pop_back();
			
				Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
				return false;
			}
			if (glm::dot(glm::cross(AD, ADB), AOrigin) > 0)
			{
				Simplex.pop_back();
				Simplex.pop_back();
				ObjectPointsA.pop_back();
				ObjectPointsA.pop_back();
			
				Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_back();
			ObjectPointsA.pop_back();
		
			Direction = ADB;
			return false;
		case(OVER_ABC|OVER_ACD):
		
			if (glm::dot(glm::cross(ABC, AC), AOrigin) > 0)
			{
				Simplex[1] = Simplex[0];
				ObjectPointsA[1] = ObjectPointsA[0];
			
				if (glm::dot(glm::cross(ACD, AC), AOrigin) > 0)
				{
					Simplex.pop_front();
					Simplex.pop_back();
					ObjectPointsA.pop_front();
					ObjectPointsA.pop_back();
				
					Direction = glm::cross(glm::cross(AC, AOrigin), AOrigin);
					return false;
				}
				if (glm::dot(glm::cross(AD, ACD), AOrigin) > 0)
				{
					Simplex[2] = Simplex[3];
					Simplex.pop_back();
					Simplex.pop_front();
					ObjectPointsA[2] = ObjectPointsA[3];
					ObjectPointsA.pop_back();
					ObjectPointsA.pop_front();
				
					
					Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
					return false;
				}
				Simplex.pop_front();
				ObjectPointsA.pop_front();
			
				Direction = ACD;
				return false;
			}
			if (glm::dot(glm::cross(AB, ABC), AOrigin) > 0)
			{
				Simplex.pop_back();
				Simplex.pop_back();
				ObjectPointsA.pop_back();
				ObjectPointsA.pop_back();
	
				Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_back();
			ObjectPointsA.pop_back();
		
			Direction = ABC;
			return false;
		case(OVER_ABC|OVER_ADB):
			if (glm::dot(glm::cross(ABC, AC), AOrigin) > 0)
			{
				Simplex[2] = Simplex[1];
				Simplex[1] = Simplex[3];
				ObjectPointsA[2] = ObjectPointsA[1];
				ObjectPointsA[1] = ObjectPointsA[3];
			
				if (glm::dot(glm::cross(ADB, AB), AOrigin) > 0)
				{
					Simplex[1] = Simplex[0];
					Simplex.pop_front();
					Simplex.pop_back();
					ObjectPointsA[1] = ObjectPointsA[0];
					ObjectPointsA.pop_front();
					ObjectPointsA.pop_back();
				
					Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
					return false;
				}
				if (glm::dot(glm::cross(AD, ADB), AOrigin) > 0)
				{
					Simplex.pop_back();
					Simplex.pop_back();
					ObjectPointsA.pop_back();
					ObjectPointsA.pop_back();
				
					Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
					return false;
				}
				Simplex.pop_back();
				ObjectPointsA.pop_back();
			
				Direction = ADB;
				return false;
			}
			if (glm::dot(glm::cross(AB, ABC), AOrigin) > 0)
			{
				Simplex.pop_back();
				Simplex.pop_back();
				ObjectPointsA.pop_back();
				ObjectPointsA.pop_back();
			
				Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
				return false;
			}
			Simplex.pop_back();
			ObjectPointsA.pop_back();
			
			Direction = ABC;
			return false;
		case(OVER_ACD|OVER_ADB):
			if (glm::dot(glm::cross(ACD, AC), AOrigin) > 0)
			{
				Simplex[2] = Simplex[1];
				Simplex[1] = Simplex[3];
				ObjectPointsA[2] = ObjectPointsA[1];
				ObjectPointsA[1] = ObjectPointsA[3];
				
				if (glm::dot(glm::cross(ADB, AB), AOrigin) > 0)
				{
					Simplex[1] = Simplex[0];
					Simplex.pop_front();
					Simplex.pop_back();
					ObjectPointsA[1] = ObjectPointsA[0];
					ObjectPointsA.pop_front();
					ObjectPointsA.pop_back();
					
					
					Direction = glm::cross(glm::cross(AB, AOrigin), AOrigin);
					return false;
				}
				if (glm::dot(glm::cross(AD, ADB), AOrigin) > 0)
				{
					Simplex.pop_back();
					Simplex.pop_back();
					ObjectPointsA.pop_back();
					ObjectPointsA.pop_back();
				
					Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
					return false;
				}
				Simplex.pop_back();
				ObjectPointsA.pop_back();
			
				Direction = ADB;
				return false;
			}
			if (glm::dot(glm::cross(AD, ACD), AOrigin) > 0)
			{
				Simplex[2] = Simplex[3];
				Simplex.pop_back();
				Simplex.pop_front();
				ObjectPointsA[2] = ObjectPointsA[3];
				ObjectPointsA.pop_back();
				ObjectPointsA.pop_front();
			
				Direction = glm::cross(glm::cross(AD, AOrigin), AOrigin);
				return false;
			}
			Simplex[1] = Simplex[0];
			Simplex.pop_front();
			ObjectPointsA[1] = ObjectPointsA[0];
			ObjectPointsA.pop_front();
		
			Direction = ACD;
			return false;
		default:
			return false;
		
	}
	
	return false;
}
glm::vec3 Collision::Dcross(const glm::vec3 &a, const  glm::vec3 &b) {
	return glm::cross(glm::cross(a,b),b);
}

void Collision::UpdateCollisionInfo(const Triangle* triangle) {

	if (!isnan(triangle->Normal.x)){
		ContactPointA = glm::vec3();
		ContactPointNormal = glm::vec3();
		PenetrationDepth = 0;
	
		float distanceFromOrigin = glm::dot(triangle->Normal, -triangle->Points[0]);//fabs((glm::dot(ABC,-A))) / (sqrt(SQR(ABC.x) + SQR(ABC.y) + SQR(ABC.z)));
			
		BarycentricCoefficents = Transform::Barycentric(distanceFromOrigin*triangle->Normal, triangle->Points[0], triangle->Points[1], triangle->Points[2]);

		glm::vec3 point((BarycentricCoefficents.x * (ObjectPointsA[0]) + (BarycentricCoefficents.y * (ObjectPointsA[1])) +	(BarycentricCoefficents.z * (ObjectPointsA[2]))));

		ContactPointA = point- triangle->Normal;
		ContactPointNormal = -triangle->Normal;
		PenetrationDepth = glm::dot(triangle->Normal, triangle->Points[0]);
	}

}
glm::vec3 Collision::Support(Collider &a, Collider &b, glm::vec3 &Direction) {
	
	glm::vec3 Point1 = a.GetFurthestPoint(Direction);
	glm::vec3 Point2 = b.GetFurthestPoint(-Direction);
	ObjectPointsA.push_front(Point1);
	Point1 = Point1 - Point2;
	return  Point1;
}
glm::vec3 Collision::EPASupport(Collider *a, Collider *b, glm::vec3 &Direction) {

	glm::vec3 Point1 = a->GetFurthestPoint(Direction);
	glm::vec3 Point2 = b->GetFurthestPoint(-Direction);
	ObjectPointsA.push_front(Point1);
	ObjectPointsA.pop_back();
	Point1 = Point1 - Point2;
	return  Point1;
}
glm::vec3 Collision::GetCollisionNormal() { return ContactPointNormal; }
GLfloat Collision::GetPenetrationDepth() { return PenetrationDepth; }
