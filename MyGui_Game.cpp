/*************************************************************************************/
/*
File Name:		MyGui_Game.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "MyGui_Game.h"
#include "MyGuiManager.h"

#include "Graphics.h"
#include "Physics.h"
#include "CoreEngine.h"
#include "WindowsSystem.h"

void MyGui_Game::LoadGameWindow(bool* _open)
{
	if (!ImGui::Begin("Game", _open))
	{
		ImGui::End();
		return;
	}

	if (MyGuiManager::isGamePaused == false && !ImGui::GetIO().WantCaptureMouse)
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);

	if (MyGuiManager::isGamePaused && ImGui::Button("Play"))
	{
		MyGuiManager::isGamePaused = false;
		SaveScene("AutoSave");
	}
	if (!MyGuiManager::isGamePaused && ImGui::Button("Stop"))
	{
		MyGuiManager::isGamePaused = true;
		LoadScene("AutoSave");
	}

	if (ImGui::Button("Toggle Fullscreen"))
	{
		WINDOWS->FullScreenSwitch();
		GRAPHICS->UpdateScreenSize();
	}

	/*auto fboptr = GRAPHICS->GetFBOTexture();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(fboptr->GetRenderID())),
				ImVec2(static_cast<float>(fboptr->GetWidth()), static_cast<float>(fboptr->GetHeight())),
				ImVec2(0,1),
				ImVec2(1,0));*/

	/*if (ImGui::IsWindowFocused() && INPUTMGR->GetKeyPressed(MVK_LBUTTON))
	{
		VEC2 mousepos{ INPUTMGR->GetMousePosition() };

		auto objectmap = MyResource.GetObjMap();

		for (auto obj : objectmap)
		{
			if (obj == nullptr)
				continue;

			std::shared_ptr<Transform> transformptr = obj->has(Transform);
			if (PHYSICS->IsCollidingWithMouse(transformptr->GetPosition(), transformptr->GetScale() * 0.5f))
			{
				std::cout << obj->GetName() << std::endl;
				std::cout << transformptr->GetScale().x << ", " << transformptr->GetScale().y << std::endl;
			}
		}
	}*/

	ImGui::End();
}

void MyGui_Game::Open()
{
}

void MyGui_Game::Update()
{
	if (MyGuiManager::show_game_window) LoadGameWindow(&MyGuiManager::show_game_window);
}

void MyGui_Game::Exit()
{
}
