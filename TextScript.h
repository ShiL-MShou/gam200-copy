/************************************************************************************ /
/*
File Name:		TextScript.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			To handle and display text on top of the text sprite


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once


#include "Script.h"
#include "Transform.h"
#include "TextRenderer.h"
#include "Component.h"

#include <vector>
#include <string>

#define SPACING 45


class TextScript : public Script
{
	std::vector<std::string> textList;

	std::shared_ptr<Transform> transformPtr;
	std::shared_ptr<TextRenderer> textRendererPtr;

public:
	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;

	void Serialize(std::string _filePath);

	virtual void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;
};
