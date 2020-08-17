#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"


class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	//void Draw(const Mesh& mesh, const Shader& shader) const;
};
