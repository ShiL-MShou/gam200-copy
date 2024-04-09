/*************************************************************************************/
/*
File Name:		MyGuiManager.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include <filesystem>
#include <functional>

#include "MyGuiManager.h"
#include "MyGui_ArcheType.h"
#include "MyGui_Menubar.h"
#include "MyGui_Performance.h"
#include "MyGui_Property.h"
#include "MyGui_Scene.h"
#include "MyGui_Game.h"
#include "MyGui_Console.h"
#include "MyGui_Gizmo.h"
#include "MyGui_TileEditor.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "..\GAM200\Vendor\ImGuizmo\ImGuizmo.h"

#include "Physics.h"
#include "Graphics.h"
#include "Input.h"
#include "CoreEngine.h"
#include "WindowsSystem.h"

static bool enable_docking = true;
static bool show_demo_window = false;

bool MyGuiManager::show_scene_editor = true;
bool MyGuiManager::show_property_editor = true;
bool MyGuiManager::show_performance_data = true;
bool MyGuiManager::show_text_input = false;
bool MyGuiManager::show_archetype_editor = true; 
bool MyGuiManager::show_game_window = true;
bool MyGuiManager::show_console = true;
bool MyGuiManager::show_gizmo = true;
bool MyGuiManager::show_tile_editor = true;

bool MyGuiManager::isGamePaused = true;

std::vector<std::unique_ptr<MyGui>> MyGuiManager::mGuiWindows;
int MyGuiManager::curr_obj_id = -1;


void MyGuiManager::MyGuiManager_Init()
{
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(WINDOWS->m_hWnd);
	ImGui_ImplOpenGL3_Init();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//ImGui::StyleColorsDark();
	MyGuiManager_InitStyle();

	AddWindows();
}

void MyGuiManager::MyGuiManager_InitStyle()
{

	//ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;
	style.ChildRounding = 5.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 0.80f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.25f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void MyGuiManager::MyGuiManager_NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(true);
}

void MyGuiManager::MyGuiManager_Update()
{
	MyGuiManager_NewFrame();

	if (show_demo_window)	ImGui::ShowDemoWindow(&show_demo_window);

	//isGamePaused = CORE->GetPausedStatus();
	CORE->SetPausedStatus(false);

	MyGuiManager_EnableDocking(&enable_docking);

	MyGuiManager_Picking();

	MyGuiManager_UpdateKeyInput();

	std::for_each(mGuiWindows.begin(), mGuiWindows.end(), std::mem_fn(&MyGui::Update));

	//CORE->SetPausedStatus(isGamePaused);

	MyGuiManager_EndFrame();
}

void MyGuiManager::MyGuiManager_EndFrame()
{
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	//MyGui_RenderFunction(draw_data);
	ImGui_ImplOpenGL3_RenderDrawData(draw_data);

	ImGuiIO& io = ImGui::GetIO();
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void MyGuiManager::MyGuiManager_Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	mGuiWindows.clear();
}

void MyGuiManager::MyGuiManager_EnableDocking(bool* _open)
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;

	ImGuiIO& io = ImGui::GetIO();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	// Docking settings
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	//ImGui::PushStyleColor(ImGuiCol_DockingPreview, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", _open, window_flags);
	ImGui::PopStyleVar();
	//ImGui::PopStyleColor();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();
}

void MyGuiManager::MyGuiManager_Picking()
{
	if (!ImGui::GetIO().WantCaptureMouse && INPUTMGR->GetKeyPressed(MVK_LBUTTON) && isGamePaused)
	{
		//VEC2 mousepos{ INPUTMGR->GetMousePosition() };

		auto objectmap = MyResource.GetObjMap();

		int selectedobj = -1;
		float currobjdepth = 2.0f;

		for (auto obj : objectmap)
		{
			if (obj == nullptr)
				continue;

			std::shared_ptr<Transform> transform = obj->has(Transform);
			std::shared_ptr<SpriteComponent> sprite = obj->has(SpriteComponent);

			if (sprite && sprite->IsActive() == false)
				continue;
			if (PHYSICS->IsCollidingWithMouse(transform->GetPosition(), transform->GetScale() * 0.5f))
			{
				if (sprite->GetDepth() < currobjdepth)
				{
					currobjdepth = sprite->GetDepth();
					selectedobj = static_cast<int>(obj->GetId());
				}
			}
		}
		if (selectedobj != -1)
		{
			curr_obj_id = static_cast<GOCId>(selectedobj);
		}
	}

}

//void MyGuiManager::MyGuiManager_Gizmo()
//{
//	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
//	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
//	static bool useSnap = false;
//	static float snap[3] = { 1.f, 1.f, 1.f };
//	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
//	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
//	static bool boundSizing = false;
//	static bool boundSizingSnap = false;
//	glm::mat4 glmobj_matrix = MyResource.GetObjectWithID(curr_obj_id)->has(Transform)->ConvertIntoMat4();
//
//	float view_matrix[16] = {};
//	const float* glm_view = static_cast<const float*>(glm::value_ptr(GRAPHICS->GetViewMatrix()));
//	for (int i = 0; i < 16; ++i)
//		view_matrix[i] = glm_view[i];
//
//	float proj_matrix[16] = {};
//	const float* glm_proj = static_cast<const float*>(glm::value_ptr(GRAPHICS->GetProjMatrix()));
//	for (int i = 0; i < 16; ++i)
//		proj_matrix[i] = glm_proj[i];
//
//	float obj_matrix[16] = {};
//	const float* glm_obj = static_cast<const float*>(glm::value_ptr(glmobj_matrix));
//	for (int i = 0; i < 16; ++i)
//		obj_matrix[i] = glm_obj[i];
//
//	if (INPUTMGR->GetKeyRelease(MVK_Z))
//		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
//	if (INPUTMGR->GetKeyRelease(MVK_X))
//		mCurrentGizmoOperation = ImGuizmo::ROTATE;
//	if (INPUTMGR->GetKeyRelease(MVK_C))
//		mCurrentGizmoOperation = ImGuizmo::SCALE;
//
//	if (INPUTMGR->GetKeyRelease(MVK_V))
//		useSnap = !useSnap;
//
//	ImGuiIO& io = ImGui::GetIO();
//	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
//	ImGuizmo::Manipulate(view_matrix, proj_matrix, mCurrentGizmoOperation, mCurrentGizmoMode, obj_matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
//}

void MyGuiManager::MyGuiManager_UpdateKeyInput()
{
	if (isGamePaused && !ImGui::GetIO().WantCaptureKeyboard)
	{
		// camera movement
		if (INPUTMGR->GetKeyHold(MVK_W))
			GRAPHICS->TestCameraY(-5.0f);
		else if (INPUTMGR->GetKeyHold(MVK_A))
			GRAPHICS->TestCameraX(5.0f);
		else if (INPUTMGR->GetKeyHold(MVK_S))
			GRAPHICS->TestCameraY(5.0f);
		else if (INPUTMGR->GetKeyHold(MVK_D))
			GRAPHICS->TestCameraX(-5.0f);
	}
}

void MyGuiManager::AddWindows()
{
	mGuiWindows.push_back(std::make_unique<MyGui_ArcheType>());
	mGuiWindows.push_back(std::make_unique<MyGui_Menubar>());
	mGuiWindows.push_back(std::make_unique<MyGui_Performance>());
	mGuiWindows.push_back(std::make_unique<MyGui_Scene>());
	mGuiWindows.push_back(std::make_unique<MyGui_Property>());
	mGuiWindows.push_back(std::make_unique<MyGui_Gizmo>());
	//mGuiWindows.push_back(std::make_unique<MyGui_Console>());
	mGuiWindows.push_back(std::make_unique<MyGui_Game>());
	mGuiWindows.push_back(std::make_unique<MyGui_TileEditor>());
}


