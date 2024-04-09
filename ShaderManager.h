/*************************************************************************************/
/*
File Name:		ShaderManager.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			ShaderManager stores the list of shaders.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <map>
#include <string>
#include <memory>

#include "Shader.h"
#include "Singleton.h"

#define MyShaderManager (ShaderManager::GetInstance())


class ShaderManager : public Singleton<ShaderManager>
{
	std::shared_ptr<Shader> shaderList[static_cast<int>(ShaderType::SHADER_MAX_NUM)];
	std::shared_ptr<Shader>	currShader;

	ShaderManager();
	~ShaderManager();

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;

	friend class Singleton<ShaderManager>;

public:
	void Initialize();

	void LoadAllShaders(std::string _filePath);
	std::shared_ptr<Shader> LoadShader(std::string _filePath, ShaderType _shaderType);

	void BindShader(ShaderType _shaderType);

	std::shared_ptr<Shader> GetShader(ShaderType _shaderType) const;
};
