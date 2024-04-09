/*************************************************************************************/
/*
File Name:		MyGuiManager.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "imgui.h"
#include "MyGui.h"
#include "Engine.h"

class MyGuiManager
{
public:
	static int curr_obj_id;
	static std::vector<std::unique_ptr<MyGui>> mGuiWindows;

	static bool show_scene_editor;
	static bool show_property_editor;
	static bool show_performance_data;
	static bool show_text_input;
	static bool show_archetype_editor;
	static bool show_game_window;
	static bool show_console;
	static bool show_gizmo;
	static bool show_tile_editor;

	static bool isGamePaused;
	
	static void MyGuiManager_Init();
	static void MyGuiManager_InitStyle();
	static void MyGuiManager_NewFrame();
	static void MyGuiManager_Update();
	static void MyGuiManager_EndFrame();
	static void MyGuiManager_Shutdown();

	static void MyGuiManager_EnableDocking(bool* _open);
	static void MyGuiManager_Picking();
	static void MyGuiManager_UpdateKeyInput();
	static void AddWindows();

};

