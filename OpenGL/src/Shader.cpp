#include <iostream>
#include <fstream>
#include <streambuf>

#include "Shader.h"
#include "GLCall.h"

Shader::Shader(const std::string& vsFilepath, const std::string& fsFilepath)
	:m_VSFilePath(vsFilepath), m_FSFilePath(fsFilepath), m_RendererID(0)
{
    // Load shader source code
    std::string vertexShader = ReadShader(vsFilepath);
    std::string fragmentShader = ReadShader(fsFilepath);

    // Create shader
    m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    // Enable our shader
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& value)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    // Check if we already have the location on our cache
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: Uniform '"<< name << "' doesn't exist." << std::endl;
    
    // Add it to our cache
    m_UniformLocationCache[name] = location;
    
	return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type)); // Create new shader and get id
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr)); // Bind the source code with our new shader
    GLCall(glCompileShader(id)); // Compile it

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) // if compilation failed
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //char* message = (char*)_malloca(length * sizeof(char));
        char* message = new char[length];
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Compilation error in " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        //free(message);
        delete[] message;

        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

std::string Shader::ReadShader(const std::string& filepath)
{
    std::ifstream in(filepath);
    if (in)
    {
        return(std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()));
    }
    throw"Couldn't open file";
}


