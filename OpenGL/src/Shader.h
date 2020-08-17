#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
private:
	std::string m_VSFilePath;
	std::string m_FSFilePath;
	unsigned int m_RendererID;

	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& vsFilepath, const std::string& fsFilepath);
	~Shader();

	void Bind() const;
	void Unbind()const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& value);

private:
	int GetUniformLocation(const std::string& name);

	std::string ReadShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};
