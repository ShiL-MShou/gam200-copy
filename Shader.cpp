/************************************************************************************ /
/*
File Name:		Shader.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Shader for graphics.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "Shader.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include "glew.h"
#include "Renderer.h"


// ----- Shader Source -----

ShaderSource::ShaderSource(std::vector<std::string> rhs)
{
	std::swap(src, rhs);
}


// ----- Shader -----

Shader::Shader()
	: m_filePath{ "" }, m_rendererID{ 0 }
{
}

Shader::Shader(const std::string& filepath, const ShaderType& _fragType)
	: m_filePath{ filepath }, m_rendererID{ 0 }
{
	ShaderSource src = ParseShader(filepath);
	
	if (_fragType == ShaderType::SHADER_IMG)
		m_rendererID = CreateShader(src.src[0], src.src[1]);
	else if (_fragType == ShaderType::SHADER_TXT)
		m_rendererID = CreateShader(src.src[0], src.src[2]);
	else if (_fragType == ShaderType::SHADER_COLOR)
		m_rendererID = CreateShader(src.src[0], src.src[3]);
	else
		assert(0); // force error
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}

Shader::Shader(const Shader& _rhs)
	: m_filePath{ _rhs.m_filePath }, m_rendererID{ _rhs.m_rendererID }
{
}

Shader& Shader::operator=(const Shader& rhs)
{
	m_filePath = rhs.m_filePath;
	m_rendererID = rhs.m_rendererID;

	return *this;
}

void Shader::Bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

ShaderSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX,
		FRAGMENT
	};

	const int FRAGMENT_MAX = 5;

	std::string line;

	std::string ss[1 + FRAGMENT_MAX];
	std::vector<std::string> ss_vec;
	ShaderType type = ShaderType::NONE;
	int fragmentNum = -1;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			// vertex
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}

			// fragment
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
				++fragmentNum;
			}
		}
		else
		{
			ASSERT(FRAGMENT_MAX >= fragmentNum);

			if (type == ShaderType::VERTEX)
				ss[static_cast<int>(type)] += (line + "\n");
			else
				ss[(int)type + fragmentNum] += (line + "\n");
		}
	}

	for (std::string s : ss)
		ss_vec.push_back(s);

	return { ss_vec };
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();					// make sure that source is not rubish value
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//char* message = (char*)alloca(length * sizeof(char)); // [length] ;
		//char* message = new char[length];

		std::unique_ptr<char[]> message(new char[length]);

		glGetShaderInfoLog(id, length, &length, &message[0]);

		std::cout << "ERROR MESSAGE " << message << std::endl;
		std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader." <<  std::endl;

		glDeleteShader(id);

		//delete[] message;

		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// creating program, vertex shader and fragment shader
	unsigned int program = glCreateProgram();
	unsigned int vertShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// attaching vertex and fragment shader to the program
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	
	glLinkProgram(program);					// read up
	glValidateProgram(program);				// read up

	// after linking/ attaching to program, we can delete tehn 
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}


void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMat4f(const std::string& name, const std::array<float, 16>& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0]);
}


int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != std::end(m_UniformLocationCache))
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_rendererID, name.c_str());

	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;

	m_UniformLocationCache[name] = location;

	return location;
}

