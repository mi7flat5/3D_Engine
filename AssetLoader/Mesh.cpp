#include"Mesh.h"
#include<GL/glew.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<vector>
#include<sstream>
#include<iostream>



void Mesh::SetMode(int in)
{
	ShaderMode = in;
	std::cout << "'\nShaderMode changed to: " << ShaderMode;
}

Mesh::Mesh(const std::vector<Vertex> &inVerts, const std::vector<GLuint> &inIndice,const std::vector<Texture> &inTextures,MaterialType Mat)
{ 
	this->verts = inVerts; 
	this->indices = inIndice; 
	this->textures = inTextures;
	setupMesh(Mat); 
}
Mesh::~Mesh() {  }

void Mesh::setupMesh(MaterialType Mat) 
{
		
	
	if (Mat != MaterialType::TEXTURE_3D)
	{
		SetTextureMode();//sets The int ShaderMode for vertex and fragment texture shader logic

		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(VAO);
	
	
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
		if (Mat == MaterialType::TEXTURE_2D_DISPLACEMENT)
		std::cout << "\nVerts size: " << indices.size();
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
	
	
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); //indices
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (GLvoid*)0);
			

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO); //Vertex Normals
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);//Diffuse color
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, diffuse));
		
		if (ShaderMode >= 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);//Texture coordinates
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
		}
		if (ShaderMode > 1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);//Tangents 
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);//BitTangents
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, BitTangent));
		}
	
	}
	else 
	{
		
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);
		
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (GLvoid*)0);
		
	}
	
	glBindVertexArray(0);
	
		
}
void Mesh::SetupCubemap()
{
}
void Mesh::drawMesh(GLuint ShaderID,MaterialType TexType) const
{
	if(TexType!=MaterialType::TEXTURE_3D)
	{
		if (TexType != MaterialType::TEXTURE_2D_REFRACT)
		{
			// Bind appropriate textures
			GLuint diffuseNr = 1;
			GLuint specularNr = 1;
			GLuint normalNr = 1;
			GLuint displaceMap =1;
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
					ss << specularNr++; 
				else if (name == "material.texture_normal")
					ss << normalNr++; 
				else if (name == "material.texture_displacement")
					ss << displaceMap++; 

				number = ss.str();
				
				// Now set the sampler to the correct texture unit
				glUniform1i(glGetUniformLocation(ShaderID, (name + number).c_str()), i);
				// And finally bind the texture
				glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
			}

			// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
			glUniform1f(glGetUniformLocation(ShaderID, "material.shininess"), 16.0f);
			glUniform1i(glGetUniformLocation(ShaderID, "Mode"), ShaderMode);//TODO remove, testing only, causes instability
		}
			// Draw mesh
			glBindVertexArray(this->VAO);
			if (TexType == MaterialType::TEXTURE_2D_DISPLACEMENT)
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawElements(GL_PATCHES, this->indices.size(), GL_UNSIGNED_INT, 0);//glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
				glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT,0 );
			glBindVertexArray(0);

			// Always good practice to set everything back to defaults once configured.
			for (GLuint i = 0; i < this->textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		
	}
	else
	{
		//Currently only for cubemaps
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0].id);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		
	}
	glBindVertexArray(0);
}
//Currently not used
void Mesh::drawMesh() const
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetTextureMode()
{
	ShaderMode = -1;
	GLuint diff = 0, spec = 0, norm = 0, disp = 0;
	for (GLuint i = 0; i < textures.size();i++)
	{
		if (textures[i].type == "material.texture_diffuse")
			diff++;
		if (textures[i].type == "material.texture_specular")
			spec++;
		if (textures[i].type == "material.texture_normal")
			norm++;
		if (textures[i].type == "material.texture_displacement")
			disp++;

	}
	if (diff > 0 && spec == 0 && norm == 0&&disp ==0)
		ShaderMode = 0;
	if (diff > 0 && spec > 0 && norm == 0 && disp == 0)
		ShaderMode = 1;
	if (diff > 0 && spec == 0 && norm > 0 && disp == 0)
		ShaderMode = 2;
	if (diff > 0 && spec > 0 && norm > 0 && disp == 0)
		ShaderMode = 3;
	if (diff > 0 && spec == 0 && norm == 0 && disp > 0)
		ShaderMode = 4;
	if (diff > 0 && spec > 0 && norm == 0 && disp > 0)
		ShaderMode = 5;
	if (diff > 0 && spec == 0 && norm > 0 && disp > 0)
		ShaderMode = 6;
	if ((diff > 0) && (spec > 0) && (norm > 0) && (disp >0))
		ShaderMode = 7;
	std::cout << "\nShader Mode: " << ShaderMode;
}