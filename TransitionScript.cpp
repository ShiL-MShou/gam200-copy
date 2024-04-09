/************************************************************************************ /
/*
File Name:		TransitionScript.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Handling the fading in and out of the screen before moving
				into another scene.

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "TransitionScript.h"

#include "SpriteComponent.h"
#include "Logic.h"

#include "Graphics.h"


TransitionScript::TransitionScript()
	: _transitionStr{}, alpha{0.0f}
{
}

TransitionScript::~TransitionScript()
{
}

void TransitionScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	if (alpha < 1.0f)
	{
		alpha += 2.0f * _dt; // alpha increment

		if (alpha > 1.0f)
			alpha = 1.0f;
		GRAPHICS->SetAlpha(1.0f - alpha);

		//gameObjPtr.lock()->has(SpriteComponent)->SetAlpha( 1.0f - alpha);
	}
	else
	{
		alpha = 0.0f;
		LOGIC->ChangeLevel(_transitionStr);
	}
}
