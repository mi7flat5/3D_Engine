#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include"Model.h"
class CubeMap : public Model {
	bool bIsDrawn;

public:
	CubeMap(const std::string &InPath, MeshType shader);
	void Draw()const ;
};
#endif // !CUBEMAP_HPP