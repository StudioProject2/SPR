
#include "MeshBuilder.h"
#include "DegreeToRadian.h"
#include <GL\glew.h>
#include "Vertex.h"
#include <vector>
#include "LoadOBJ.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	for (int theta = 0; theta <= 360; theta += 10)
	{
		float x = 10 * cosf(DegreeToRadian(theta));
		float y = 10 * sinf(DegreeToRadian(theta));

		v.pos.Set(x, 0, y);
		v.color.Set(1, 0, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, 0, 0);
		v.color.Set(1, 0, 0);
		vertex_buffer_data.push_back(v);
	
	}

	std::vector<GLuint> index_buffer_data;

	for (int i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}
	
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad1(const std::string &meshName, Color color, float lengthX, float lengthY, float textRatio)
{
	std::vector<Vertex> vertex_buffer_data;
	Vertex v;

	v.pos.Set(-lengthX, lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, textRatio);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-lengthX, -lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(textRatio, textRatio);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(textRatio, textRatio);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-lengthX, -lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, -lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(textRatio, 0);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;

	for (int i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY)
{
	std::vector<Vertex> vertex_buffer_data;
	Vertex v;

	v.pos.Set(-lengthX, lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-lengthX, -lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-lengthX, -lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(lengthX, -lengthY, 0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;

	for (int i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	std::vector<Vertex> vertexBuffer;
	std::vector<GLuint> indexBuffer;


	Vertex v;
	v.pos.Set(-0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//3

	v.pos.Set(0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//6

	v.pos.Set(0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//9

	v.pos.Set(0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//12

	v.pos.Set(0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//15

	v.pos.Set(0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//18

	v.pos.Set(-0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//21

	v.pos.Set(-0.5f, 0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//24

	v.pos.Set(-0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//27

	v.pos.Set(-0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, 0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//30

	v.pos.Set(-0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(-0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);
	//33

	v.pos.Set(-0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, 0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	v.pos.Set(0.5f, -0.5f, -0.5f);
	v.color.Set(1.0f, 0.0f, 0.0f);
	vertexBuffer.push_back(v);

	for (int i = 0; i < 36; i++)
	{
		indexBuffer.push_back(i);
	}
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); //bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(GLuint), &indexBuffer[0], GL_STATIC_DRAW);

	mesh->indexSize = indexBuffer.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}


Mesh* MeshBuilder::GenerateRing(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;


	for (int theta = 0; theta <= 360; theta += 10)
	{
		float x = 10 * cosf(DegreeToRadian(theta));
		float y = 10 * sinf(DegreeToRadian(theta));

		v.pos.Set(1.2*x, 0, 1.2*y);
		v.color.Set(1, 0, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(1 * x, 0, 1*y);
		v.color.Set(2, 2, 0);
		vertex_buffer_data.push_back(v);

	}

	std::vector<GLuint> index_buffer_data;

	for (int i = 0; i < vertex_buffer_data.size(); ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

float SphereX(float phi, float theta)
{
	return cos(DegreeToRadian(phi)) * cos(DegreeToRadian(theta));
}

float SphereY(float phi, float theta)
{
	return sin(DegreeToRadian(phi));
}

float SphereZ(float phi, float theta)
{
	return cos(DegreeToRadian(phi)) * sin(DegreeToRadian(theta));
}
Mesh* MeshBuilder::GenerateHem(const std::string &meshName, Color colour, unsigned numstack, unsigned numslice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;


	float degreePerStack = 180.f / numstack;
	float degreePerSlice = 360.f / numslice;

	for (unsigned stack = 0; stack < numstack + 1; ++stack)
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < numslice + 1; ++slice)
		{
			float theta = slice *degreePerSlice;
			Vertex v;
			v.pos.Set(radius * SphereX(phi, theta), radius * SphereY(phi, theta), radius * SphereZ(phi, theta));
			v.color = colour;
			v.normal.Set(SphereX(phi, theta), SphereY(phi, theta), SphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numstack + 1; ++stack)
	{
		for (unsigned slice = 0; slice < numslice + 1; ++slice)
		{
			index_buffer_data.push_back((numslice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numslice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path) {
	//Read vertices, texcoords & normals from OBJ

	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
	
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;
	int offset = 0;
 
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	for (unsigned j = 0; j < numRow; ++j)
	{
		for (unsigned i = 0; i < numCol; ++i)
		{
			float u1 = i * width;
			float v1 = 1.f - height - j * height;
			
			v.pos.Set(-0.5, -0.5, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5, 0.5, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, 0.5, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, -0.5, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);

			offset = offset + 4;

		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

