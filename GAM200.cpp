/*************************************************************************************/
/*
File Name:		GAM200.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
// GAM200.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "GAM200.h"
//#include "Engine.h"
//#include "WindowsSystem.h"
//#include "Graphics.h"
#include "CoreEngine.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#include "Graphics.h"

//#include <ctime>			// for dt, (clock_t)
#include <iostream>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR,
	_In_ int)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
			// allocations to be of _CLIENT_BLOCK type
	#else
	#define DBG_NEW new
	#endif

	#ifdef _DEBUG
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitleW(L"Debug Window");

		std::cout << "Game Starting..." << std::endl;
	}
	#endif

	CORE = std::make_unique<CoreEngine>();
	CORE->Init(hInstance);
	// add the systems into the engine
	CORE->AddSystems();
	// init the systems in the engine
	CORE->InitSystem();
	//WINDOWS->WindowInit(nCmdShow);
	// updates the systems in the engine
	CORE->Update();
	// destroy the systems in the engine
	CORE->DestroySystems();

	//GRAPHICS->Exit();
	return 0;
}
