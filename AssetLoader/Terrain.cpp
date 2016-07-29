#include"Terrain.hpp"
#include"LoadUtility.h"
#include"include\SOIL\SOIL.h"


Terrain::Terrain() {}
Terrain::Terrain(const std::string &InPath, MeshType Mat) : Model(InPath, Mat) {
	
	HeightMapImage = LoadUtility::loadHeightMap("media/hm_d.jpg", Width, Height);
	ModelShader = new Shaders("shaders/dispmap.vs.glsl",
		"shaders/dispmap.tcs.glsl",
		"shaders/dispmap.tes.glsl",
		"shaders/dispmap.fs.glsl");
	NormMappingID = glGetUniformLocation(ModelShader->getProgram(), "NormMapping");
	DispLevelID = glGetUniformLocation(ModelShader->getProgram(), "DispLevel");
	SpecularShininessID = glGetUniformLocation(ModelShader->getProgram(), "material.shininess");
	ProjectionMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Projection");
	ViewMatrixID = glGetUniformLocation(ModelShader->getProgram(), "View");
	ModelMatrixID = glGetUniformLocation(ModelShader->getProgram(), "Model");
	for (GLuint i = 0; i < meshes.size();i++)
		meshes[i].SetShader(ModelShader->getProgram());
	
	SetScale(50);
}
Terrain::~Terrain() {
	
	if(HeightMapImage)
		SOIL_free_image_data(HeightMapImage);
}
void Terrain::SetMinMaxBoundry()
{
	MaxZ = (32 * Scale * (64 + Height)) / Height;
	MaxX = (32 * Scale * (64 + Width)) / Width;

	MinX = -MaxX;
	MinZ = -MaxZ;

	MaxX = MaxX - Scale;
	MaxZ = MaxZ - Scale;
}
void Terrain::SetScale(GLuint InScale) {
	Scale = InScale;
	TerrainModel = glm::mat4();
	TerrainModel[0][0] = Scale;
	TerrainModel[1][1] = 1.0;
	TerrainModel[2][2] = Scale;
	TerrainModel[3][1] = 0;
	TerrainModel = glm::transpose(TerrainModel);
	SetMinMaxBoundry();
}
GLuint Terrain::GetHeight(glm::vec3 Position)
{
	GLuint XLow = floor(Position.x * (Width / 2) / (32 * Scale) + 32 + (Width / 2));
	GLuint ZLow = floor(Position.z * (Height / 2) / (32 * Scale) + 32 + (Height / 2));
		
	if (XLow < 1 || ZLow <1)
		return LastValidHeight;
	//GLuint wraps at 0
	if ((XLow > Width - 2) || (ZLow > Height - 2))
		return LastValidHeight;
	
	LastValidHeight = (GLuint)HeightMapImage[((ZLow*Width) + XLow) * 3];
	return LastValidHeight;
}
void Terrain::Draw()const
{
	ModelShader->Use();
	glUniform1f(DispLevelID, DispLevel);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &SHCamera::Projection[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &TerrainModel[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &SHCamera::View[0][0]);
	for (int i = 0;i < meshes.size();i++)
	{
		meshes[i].DrawMesh(ModelShader->getProgram(), ModelMaterial);
	}
}

GLfloat Terrain::GetMaxX() const
{
	return MaxX-1;
}
GLfloat Terrain::GetMaxZ() const
{
	return MaxZ-1;
}
GLfloat Terrain::GetMinX() const
{
	return MinX;
}
GLfloat Terrain::GetMinZ() const
{
	return MinZ;
}




