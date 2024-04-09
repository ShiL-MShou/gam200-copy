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


#include "stdafx.h"
#include "TextScript.h"

#include "Graphics.h"


void TextScript::Start()
{
}

void TextScript::Init()
{
	textRendererPtr = GRAPHICS->GetTextRenderer();
}

void TextScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (gameObjPtr.lock()->has(SpriteComponent)->IsActive() == false)
		return;

	glm::mat4 vp = GRAPHICS->GetProjMatrix();

	VEC2 startPt(100, 175);

	for (auto text : textList)
	{
		textRendererPtr->AddText(text, startPt);

		startPt.y -= SPACING;
	}

}

void TextScript::Exit()
{
}

void TextScript::Serialize(std::string _filePath)
{
	UNREFERENCED_PARAMETER(_filePath);

	textList.clear();

	JsonReader jr(_filePath);
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened);

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		std::cout << "Error Loading Text Script!" << std::endl;
		assert(0);
		return;
	}
	else
	{
		// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}


	std::string jsonKey1 = "TextScript";

	std::string text;

	std::string tmpKey;

	int i = 1;

	while (jr.IsGood())
	{
		tmpKey = jsonKey1 + std::to_string(i);

		if (jr.doc.HasMember(tmpKey.c_str()))
		{
			jr.ReadStringMember(tmpKey, "Text", text);

			textList.push_back(text);

			std::cout << "TextScript loaded: " << text << std::endl;
		}
		else
			break;

		++i;
	}
}

void TextScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
	transformPtr = _gameObjPtr->has(Transform);
}
