/*************************************************************************************/
/*
File Name:		ShaderManager.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			ShaderManager stores the list of shaders.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
}

void ShaderManager::LoadAllShaders(std::string _filePath)
{
	shaderList[static_cast<int>(ShaderType::SHADER_IMG)]	= LoadShader(_filePath, ShaderType::SHADER_IMG);
	shaderList[static_cast<int>(ShaderType::SHADER_TXT)]	= LoadShader(_filePath, ShaderType::SHADER_TXT);
	shaderList[static_cast<int>(ShaderType::SHADER_COLOR)]	= LoadShader(_filePath, ShaderType::SHADER_COLOR);
}

std::shared_ptr<Shader> ShaderManager::LoadShader(std::string _filePath, ShaderType _shaderType)
{
	std::shared_ptr<Shader> tmpShaderPtr = std::make_shared<Shader>(_filePath, _shaderType);

	if (tmpShaderPtr)
		return tmpShaderPtr;
	else
	{
		assert(0); // force assert
		std::cout << "ERROR: Shader failed to load: " << _filePath << "-" << (int)_shaderType << std::endl;
		return nullptr;
	}
}

void ShaderManager::BindShader(ShaderType _shaderType)
{
	shaderList[static_cast<int>(_shaderType)]->Bind();
}

std::shared_ptr<Shader> ShaderManager::GetShader(ShaderType _shaderType) const
{
	return shaderList[static_cast<int>(_shaderType)];
}
