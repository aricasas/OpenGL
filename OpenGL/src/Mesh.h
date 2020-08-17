#pragma once

#include "glm/glm.hpp"

class Mesh
{
public:
	Mesh(const char* filepath, glm::vec3 position);
	~Mesh();

	glm::vec3 position;

	void Draw() const;

};

