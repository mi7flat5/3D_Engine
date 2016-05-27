#include"Mesh.h"
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vector>
#include<sstream>



Mesh::Mesh(const std::vector<Vertex> &inVerts, const std::vector<GLuint> &inIndice,const std::vector<Texture> &inTextures) 
{ 
	this->verts = inVerts; 
	this->indices = inIndice; 
	this->textures = inTextures;
	setupMesh(); 
}
Mesh::~Mesh() {  }

void Mesh::setupMesh() 
{
	
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(VAO);
	
		
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  
		3,                  // Components
		GL_FLOAT,           
		GL_FALSE,           
		sizeof(Vertex),     // stride
		(void*)0);			//Pointer offset

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); //indices
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (GLvoid*)0);
		

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO); //Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,3, GL_FLOAT,GL_TRUE,sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);//Diffuse color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)offsetof(Vertex, diffuse));

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);//Diffuse color
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	
	glBindVertexArray(0);

}
void Mesh::drawMesh(GLuint UnModel) const
{
	{
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			std::stringstream ss;
			std::string number;
			std::string name = this->textures[i].type;
			if (name == "material.texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "material.texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();
			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(UnModel,(name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}

		// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(UnModel, "material.shininess"), 16.0f);

		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}
void Mesh::drawMesh() const
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}