/*************************************************************************************/
/*
File Name:		CoreEngine.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "CoreEngine.h"
#include "WindowsSystem.h"
#include "Graphics.h"
#include "Logic.h"
#include "Physics.h"
#include "Input.h"
#include "TextSerialization.h"
#include "MyGuiManager.h"
#include "FModSoundManager.h"
#include "Score.h"

#include "TimeManager.h"


std::unique_ptr<CoreEngine> CORE;

const float fixedDeltaTime = 1.0f / 60.0f;
float accumulatedTime = 0.0f;
double mSysStartTime = 0.0;

// Global Width and Height of Window size
int WinWidth;
int WinHeight;

CoreEngine::CoreEngine() : m_lastTime{ 0 }, m_gameActive{ true }, mCPUTime{ 0 }, currNumSteps{ 0 }, mIsGamePaused{ false }, m_isFullScreen{ false }
{
}

CoreEngine::~CoreEngine()
{
}

void CoreEngine::Init(HINSTANCE _hInst)
{
	bool vsync = InitConfig();

	WINDOWS = std::make_shared<WindowsSystem>(_hInst);
	PHYSICS = std::make_shared<Physics>();
	LOGIC = std::make_shared<Logic>();
	INPUTMGR = std::make_shared<Input>();
	SOUNDMGR = std::make_shared<FModSoundManager>();
	FACTORY = std::make_shared<Factory>();
	GRAPHICS = std::make_shared<Graphics>(WINDOWS->m_hWnd);

	SetVSync(vsync);
}

void CoreEngine::Update()
{
	m_lastTime = static_cast<float>(clock());

	while (m_gameActive)
	{
		// reset the num of steps
		currNumSteps = 0;
		//Get the current time in milliseconds
		float currentTime = static_cast<float>(clock());

		//Convert it to the time passed since the last frame (in seconds)
		m_dt = (currentTime - m_lastTime) / (float)CLOCKS_PER_SEC;
		mCPUTime = static_cast<double>(currentTime) - static_cast<double>(m_lastTime);
		accumulatedTime += m_dt;

		MyTimeManager.Update(m_dt);

		while (accumulatedTime >= fixedDeltaTime)
		{
			accumulatedTime -= fixedDeltaTime;
			++currNumSteps;
		}
		
		//Update the when the last update started
		m_lastTime = currentTime;

		if (mCurrLevel != mNextLevel)
		{
			Clear();
			if (strcmp(mNextLevel.c_str(), "AutoSave"))
				CORE->SetCurrLevel(mNextLevel);
			LOGIC->LoadLevel(mNextLevel);
		}

		//Update every system and tell each one how much
		//time has passed since the last update
		for (auto& iter : m_systems)
		{
			SystemStartUpdate();
			iter->Update(fixedDeltaTime);
			SystemEndUpdate(iter->GetName());
		}

		SystemStartUpdate();
		if (mIsEditorActive)
			MyGuiManager::MyGuiManager_Update();
		SystemEndUpdate("ImGui");

		GRAPHICS->SwapBuffers();
	}
}

void CoreEngine::Clear()
{
	for (auto& iter : m_systems)
		iter->Clear();
}

bool CoreEngine::InitConfig()
{
	TextSerialization stream;
	bool isFileOpened = stream.OpenFile("../Config.txt");
	assert(isFileOpened);
	UNREFERENCED_PARAMETER(isFileOpened);

	bool vsync = true;

	while (stream.IsGood())
	{
		StreamRead(stream, WinWidth);
		StreamRead(stream, WinHeight);
		StreamRead(stream, vsync);
		StreamRead(stream, mIsEditorActive);
		StreamRead(stream, m_isFullScreen);
		StreamRead(stream, mCurrLevel);
		StreamRead(stream, mPath);
	}

	if (mIsEditorActive)
		ShowCursor(true);
	else
		ShowCursor(false);
	mNextLevel = mCurrLevel;

	return vsync;
}

void CoreEngine::InitSystem()
{
	for (auto& iter : m_systems)
	{
		iter->Initialize();
		system_Performance[iter->GetName()] = 0;
	}

	MyEvent.Initialize();

	LOGIC->LoadLevel(mCurrLevel);

	LOGIC->SaveLevel("TestLevelSave");

	MyGuiManager::MyGuiManager_Init();
	system_Performance["ImGui"] = 0;

	if (m_isFullScreen)
	{
		WINDOWS->FullScreenSwitch();
		GRAPHICS->UpdateScreenSize();
	}

	MyTimeManager.Initialize();
}

void CoreEngine::AddSystems()
{
	m_systems.push_back(WINDOWS);
	m_systems.push_back(INPUTMGR);
	m_systems.push_back(PHYSICS);
	m_systems.push_back(LOGIC);
	m_systems.push_back(SOUNDMGR);
	m_systems.push_back(FACTORY);
	m_systems.push_back(GRAPHICS);
}

void CoreEngine::DestroySystems()
{
	MyGuiManager::MyGuiManager_Shutdown();
	m_systems.clear();
}

void CoreEngine::BroadcastMessage(Message& _message)
{
	if (_message.m_Mid == MessageId::Quit)
		m_gameActive = false;
	for (auto& iter : m_systems)
		iter->SendMessage(_message);
}

void CoreEngine::SystemStartUpdate()
{
	mSysStartTime = static_cast<double>(clock());
}

void CoreEngine::SystemEndUpdate(const std::string& _systemname)
{
	system_Performance[_systemname] = static_cast<double>(clock()) -mSysStartTime;
}

void CoreEngine::ToggleEditorStatus()
{
	if (mIsEditorActive)
	{
		// hide window cursor when editor is inactive
		while (ShowCursor(false) >= 0)
			ShowCursor(false);
	}
	else
	{
		// display window cursor when editor is active
		while (ShowCursor(true) < 0)
			ShowCursor(true);
	}
	
	mIsEditorActive = !mIsEditorActive; 
}

void CoreEngine::SetPausedStatus(bool _status)
{
	mIsGamePaused = _status;
}

void CoreEngine::SetVSync(bool _sync)
{
	// turn off vSync
	typedef bool (APIENTRY * PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalExt = 0;
	wglSwapIntervalExt = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalExt(_sync);
}



