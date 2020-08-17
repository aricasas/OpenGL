#include "Texture.h"
#include "GLCall.h"
#include "stb_image/stb_image.h"


Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_FilePath(filepath), m_LocalBuffer(nullptr), // Initializing variables
	m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); // flip vertically because OpenGL considers the bottom left (0, 0) but PNGs start from the top left
	
	// load image
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);


	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Send data to OpenGL
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	// Unbind texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// delete image from memory
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
	
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
