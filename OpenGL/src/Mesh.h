#pragma once

#include "glm/glm.hpp"

#include "VertexArray.h"
#include "IndexBuffer.h"

class Mesh
{
private:
	VertexArray va;
	IndexBuffer* ib;

	bool LoadOBJFile(const char* filepath);

public:
	Mesh(const char* filepath, glm::vec3 position);

	glm::vec3 worldCoordinates;

	void Draw() const;

};

 