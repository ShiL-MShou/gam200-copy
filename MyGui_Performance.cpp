/*************************************************************************************/
/*
File Name:		MyGui_Performance.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "MyGui_Performance.h"
#include "MyGuiManager.h"

#include "CoreEngine.h"
#include "Graphics.h"

void MyGui_Performance::Open()
{
}

void MyGui_Performance::Update()
{
	if (MyGuiManager::show_performance_data)	LoadPerformanceData(&MyGuiManager::show_performance_data);
}

void MyGui_Performance::Exit()
{
}

void MyGui_Performance::LoadPerformanceData(bool* _open)
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Performance Data", _open))
	{
		ImGui::End();
		return;
	}

	for (auto& t : CORE->system_Performance)
	{
		double time = (CORE->mCPUTime > 0.0) ? (t.second / CORE->mCPUTime) : 0;
		ImGui::ProgressBar(static_cast<float>(time), ImVec2(0.0f, 0.0f));
		ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::Text(t.first.c_str());
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	VEC2 mousepos = INPUTMGR->GetMousePosition();
	//GRAPHICS->ScreenToWorld(VEC2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y));
	ImGui::Text("Mouse pos: (%g, %g)", mousepos.x, mousepos.y);
	VEC2 worldpos = GRAPHICS->ScreenToWorld(VEC2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y));
	ImGui::Text("World pos : (%g, %g)", worldpos.x, worldpos.y);


	ImGui::End();
}

