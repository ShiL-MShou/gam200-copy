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

#pragma once

#include <string>
#include <unordered_map>

#include "glm.hpp"

struct ShaderSource
{
	std::vector<std::string> src;

	ShaderSource(std::vector<std::string> rhs);
};

enum class ShaderType : int
{
	SHADER_IMG = 0,
	SHADER_TXT,
	SHADER_COLOR,
	SHADER_MAX_NUM
};


class Shader
{
	std::string m_filePath;	// for debug

	unsigned int m_rendererID;
	//caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader();
	Shader(const std::string& filepath, const ShaderType& _fragType);
	~Shader();

	Shader(const Shader& _rhs);
	Shader& operator=(const Shader& _rhs);

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix); 
	void SetUniformMat4f(const std::string& name, const std::array<float, 16> & matrix);

	//void SetUniformMat4f(const std::string& name, const float* matrix);

	// tmp
	unsigned int GetID() { return m_rendererID; }

private:
	ShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};

