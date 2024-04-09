/*************************************************************************************/
/*
File Name:		WindowsSystem.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "WindowsSystem.h"
#include "Graphics.h"
#include "CoreEngine.h"
#include "MyGuiManager.h"
#include "Logic.h"
#include "PauseMenuScript.h"
#include "FModSoundManager.h"


#define MAX_LOADSTRING 100

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

WINDOWPLACEMENT wpc;
LONG HWNDStyle = 0;
LONG HWNDStyleEx = 0;

//A global pointer to the windows system
std::shared_ptr<WindowsSystem> WINDOWS;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{

	case WM_DROPFILES:
	{
		unsigned int itemCount = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, 0, 0);
		if (itemCount)
		{
			char buffer[512] = { 0 };
			DragQueryFileA((HDROP)wParam, 0, buffer, 512);
			DragFinish((HDROP)wParam);

			WINDOWS->HandleFileDrop(buffer);
		}
		return 0;
	}
	break;
	case WM_SETFOCUS:
	{
		if (WINDOWS->outoffocus && CORE->isFullScreen() && WINDOWS->isWindowMode)
		{
			WINDOWS->FullScreenSwitch();
			GRAPHICS->UpdateScreenSize();
		}
		SOUNDMGR->UnPauseAllSounds();
		WINDOWS->outoffocus = false;
	}
		break;

	case WM_KILLFOCUS:
	{
		if (!WINDOWS->isWindowMode)
		{
			WINDOWS->FullScreenSwitch();
			GRAPHICS->UpdateScreenSize();
		}
		WINDOWS->outoffocus = true;
		if (!CORE->GetEditorStatus() && (strcmp(CORE->GetCurrLevel().c_str(), "Stage1v2") == 0 || strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0 || strcmp(CORE->GetCurrLevel().c_str(), "Stage3v2") == 0 || strcmp(CORE->GetCurrLevel().c_str(), "Stage4") == 0|| strcmp(CORE->GetCurrLevel().c_str(), "Result") == 0))
			LOGIC->GetScript<PauseMenuScript>("PAUSEMENU_SCRIPT")->CreateMenu();
		else
		{
			CORE->SetPausedStatus(true);
		}
		SOUNDMGR->PauseAllSounds();
	}
		//SOUNDMGR->MuteMasterVolume();
		break;
	case WM_SIZE:
		WinWidth = LOWORD(lParam);
		WinHeight = HIWORD(lParam);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAM200));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

WindowsSystem::WindowsSystem(HINSTANCE _hInst) : m_hInst{ _hInst }, m_hWnd{ NULL }, isWindowMode{ true }, outoffocus{ false }
{
	// Initialize strings
	LoadStringW(m_hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(m_hInst, IDC_GAM200, szWindowClass, MAX_LOADSTRING);

	RECT fullWinRect = { 0, 0, WinWidth, WinHeight };
	AdjustWindowRect(&fullWinRect,			
		WS_OVERLAPPEDWINDOW,	 
		FALSE);					

	MyRegisterClass(m_hInst);


	//Create the game's window
	m_hWnd = CreateWindowW(szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			fullWinRect.right - fullWinRect.left,
			fullWinRect.bottom - fullWinRect.top,
			GetDesktopWindow(),
			NULL,
			m_hInst,
			NULL);

	DragAcceptFiles(m_hWnd, true);
}

WindowsSystem::~WindowsSystem()
{
	//Unregister the window class
	UnregisterClass(szWindowClass, m_hInst);
}

void WindowsSystem::Initialize()
{
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}

void WindowsSystem::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	MSG msg;

	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);	

		//If we get a quit message, broadcast it to all systems
		if (msg.message == WM_QUIT)
		{
			MessageQuit message;
			CORE->BroadcastMessage(message);
		}
	}

}

void WindowsSystem::Clear()
{
}



void WindowsSystem::HandleFileDrop(const std::string& _filename)
{
	std::string file = _filename.substr(_filename.find_last_of('\\') + 1, std::string::npos);
	std::string filename = file.substr(0, file.find_last_of('.'));
	std::string extension = file.substr(file.find_first_of('.') + 1, std::string::npos);
	std::string path = CORE->mPath + "";

	if (strcmp(extension.c_str(), "png") == 0)
	{
		path += "Images/";
		std::ifstream infile(path + file);
		if (infile.good())
		{
			std::string errormsg = file + " already exists";
			MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
		}
		else
		{
			CopyFileA(_filename.c_str(), (path + file).c_str(), TRUE);
			GRAPHICS->LoadNewAsset();
			std::string confirmmsg = file + " successfully loaded";
			MessageBoxA(NULL, confirmmsg.c_str(), "Success", MB_OK);
		}
	}
	else if (strcmp(extension.c_str(), "ogg") == 0)
	{
		path += "Sounds/";
		std::ifstream infile(path + file);
		if (infile.good())
		{
			std::string errormsg = file + " already exists";
			MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
		}
		else
		{
			CopyFileA(_filename.c_str(), (path + file).c_str(), TRUE);
			SOUNDMGR->LoadSound(filename, path + file);
			std::string confirmmsg = file + " successfully loaded";
			MessageBoxA(NULL, confirmmsg.c_str(), "Success", MB_OK);
		}
	}
	else
	{
		std::string errormsg = file + " is not in a valid art or audio format";
		MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
	}

}

void WindowsSystem::FullScreenSwitch()
{
	if (isWindowMode)
	{
		isWindowMode = FALSE;
		GetWindowPlacement(m_hWnd, &wpc);
		if (HWNDStyle == 0)
			HWNDStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		if (HWNDStyleEx == 0)
			HWNDStyleEx = GetWindowLong(m_hWnd, GWL_EXSTYLE);

		LONG NewHWNDStyle = HWNDStyle;
		NewHWNDStyle &= ~WS_BORDER;
		NewHWNDStyle &= ~WS_DLGFRAME;
		NewHWNDStyle &= ~WS_THICKFRAME;

		LONG NewHWNDStyleEx = HWNDStyleEx;
		NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

		SetWindowLong(m_hWnd, GWL_STYLE, NewHWNDStyle | WS_POPUP);
		SetWindowLong(m_hWnd, GWL_EXSTYLE,	HWNDStyleEx | WS_EX_TOPMOST);
		ShowWindow(m_hWnd, SW_SHOWMAXIMIZED);
	}
	else
	{
		isWindowMode = TRUE;
		SetWindowLong(m_hWnd, GWL_STYLE, HWNDStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, HWNDStyleEx);
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		SetWindowPlacement(m_hWnd, &wpc);
	}
}

