/*************************************************************************************/
/*
File Name:		WindowsSystem.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Engine.h"
#include "Resource.h"


class WindowsSystem : public ISystem
{
public:
	WindowsSystem(HINSTANCE _hInst);	//The constructor
	~WindowsSystem();															//The destructor

	//void WindowInit(int nCmdShow);								//Activate the game window so it is actually visible
	void Initialize();
	virtual void Update(float _dt) override;						//Update the system every frame
	virtual void Clear() override;
	virtual std::string GetName() override { return "Windows"; }	//Get the string name of the system

	void HandleFileDrop(const std::string& _filename);
	void FullScreenSwitch();

	bool isWindowMode;
	HWND m_hWnd;											//The handle to the game window
	HINSTANCE m_hInst;								//The handle to the instance

	bool outoffocus;
	//POINTS MousePosition;
};

extern std::shared_ptr<WindowsSystem> WINDOWS;

// Message signaling that a key is pressed.
class MessageCharacterKey : public Message
{
public:
	MessageCharacterKey() : Message(MessageId::CharacterKey), Character{1} {}
	int Character;
};

// Message signaling that the mouse has moved.
class MouseMove : public Message
{
public:
	MouseMove(VEC2 position) : Message(MessageId::MouseMove), MousePosition(position) {};
	VEC2 MousePosition;
};

///Message signaling that a file was dropped onto the window.
class FileDrop : public Message
{
public:
	FileDrop(std::string filename) : Message(MessageId::FileDrop), FileName(filename) {};
	std::string FileName;
};