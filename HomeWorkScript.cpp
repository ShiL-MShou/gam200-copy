/*****************************************************************************/
/*
File Name:		HomeWorkScript.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan	 [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"

#include "Logic.h"
#include "PlayerScript.h"
#include "HomeWorkScript.h"
#include "LogicComponent.h"
#include "TextScript.h"
#include "CoreEngine.h"
#include "FModSoundManager.h"

bool HWcopied = false;

void HomeWorkScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (std::shared_ptr<GOC> homework = gameObjPtr.lock())
	{
		if (homework->has(BoxCollider)->IsCollided())
		{
			// creates interactable feedback
			//interactableFeedback.Create(gameObjPtr.lock()->has(Transform)->GetPosition() + VEC2(0.0f, 45.0f));
			
			if (INPUTMGR->GetKeyRelease(MVK_E))
			{
				if (!HWcopied)
				{
					if (strcmp(homework->GetName(), "Homework") == 0) //actual goal condition
					{

						CORE->SetPausedStatus(true);

						//dialog box, player sprite, tint
						textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Homework Text");
						boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
						tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");

						//sound
						SOUNDMGR->PlaySoundFromMap("boyVoice");
						//get the dialog from the json
						std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/BoyComments/CopyHW.json");

						inDialog = true;
						playHWText = false;
						HWcopied = true;
						isActivated = true;

					}
					else if (strcmp(homework->GetName(), "HWBlank") == 0) //comment about how the page is blank
					{

						CORE->SetPausedStatus(true);
						scoreNum += 10;
						//dialog box, player sprite, tint
						textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Homework Text");
						boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
						tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");

						//sound
						SOUNDMGR->PlaySoundFromMap("boyVoice");
						//get the dialog from the json
						std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/BoyComments/HWBlank.json");
						inDialog = true;

					}
					else if (strcmp(homework->GetName(), "HWMessy") == 0) //comment on how messy the HW is
					{

						CORE->SetPausedStatus(true);
						scoreNum += 10;

						//dialog box, player sprite, tint
						textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Homework Text");
						boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
						tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");

						//sound
						SOUNDMGR->PlaySoundFromMap("boyVoice");
						//get the dialog from the json
						std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/BoyComments/HWMessy.json");
						inDialog = true;

					}
					else if (strcmp(homework->GetName(), "HWCopy1")==0)
					{
						CORE->SetPausedStatus(true);
						scoreNum += 10;

						//dialog box, player sprite, tint
						textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Homework Text");
						boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
						tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");

						//sound
						SOUNDMGR->PlaySoundFromMap("boyVoice");
						//get the dialog from the json
						std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/BoyComments/HWCopy1.json");
						inDialog = true;
					}
					else if (strcmp(homework->GetName(), "HWCopy2") == 0)
					{
						CORE->SetPausedStatus(true);
						scoreNum += 10;

						//dialog box, player sprite, tint
						textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Homework Text");
						boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
						tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");

						//sound
						SOUNDMGR->PlaySoundFromMap("boyVoice");
						//get the dialog from the json
						std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/BoyComments/HWCopy2.json");
						inDialog = true;
					}
				}
				else
				{
					CORE->SetPausedStatus(true);

					//dialog box, player sprite, tint
					textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Homework Text");
					boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
					tintObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");

					//sound
					SOUNDMGR->PlaySoundFromMap("boyVoice");
					//get the dialog from the json
					std::dynamic_pointer_cast<TextScript>(LOGIC->GetScript(textObjPtr.lock()->has(LogicComponent)->GetScriptName()))->Serialize(CORE->mPath + "Text/BoyComments/HWDone.json");
					inDialog = true;
				}
			}

			// reset collider
			homework->has(BoxCollider)->ResetIsCollide();
		}
		else
		{
			// destroys interactables feedback
			//interactableFeedback.Destroy();
		}

		if (std::shared_ptr<GOC> text = textObjPtr.lock())
		{//we're pressing enter/lmb to get out of the dialog
			if (INPUTMGR->GetKeyPressed(MVK_RETURN) || INPUTMGR->GetKeyPressed(MVK_SPACE) || INPUTMGR->GetKeyPressed(MVK_LBUTTON))
			{

				CORE->SetPausedStatus(false);

				if (std::shared_ptr<GOC> boySpr = boySpritePtr.lock())
					boySpr->Destroy();
				if (std::shared_ptr<GOC> tint = tintObjPtr.lock())
					tint->Destroy();

				inDialog = false; //no longer in dialog
				playHWText = true;
				text->Destroy();
			}
		}

		//interactableFeedback.Update(_dt);
	}
	
}

void HomeWorkScript::Start()
{
	playHWText = true;
}

void HomeWorkScript::Init()
{

}

void HomeWorkScript::Exit()
{
	playHWText = true;
	HWcopied = false;
	isActivated = false;
}

void HomeWorkScript::LoadResource()
{
	// loading resources
	//FACTORY->CreateArcheType("InteractableFeedback");

	//interactableFeedback.LoadResource();
	//FACTORY->CreateArcheType("PlayerSprite");
	//FACTORY->CreateArcheType("TeacherSprite");
	//FACTORY->CreateArcheType("Tint");
}

//void HomeWorkScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
//{
//	gameObjPtr = _gameObjPtr;
//}
