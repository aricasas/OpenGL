#include "VertexArray.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); //Bind our Vertex Array
	vb.Bind(); // Bind the buffer

	// Set up layout
	unsigned int offset = 0;

	const std::vector<VertexBufferElement> elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		// Enabling our vertex attributes
		const VertexBufferElement element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset)); // describes the format of our vertices

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type); // Add the size of the element to the offset
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
