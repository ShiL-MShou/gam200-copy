/******************************************************************************/
/*
	File Name: Input.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Input system keeps track of the state of the keyboard

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "Input.h"
#include "WindowsSystem.h"
#include <iostream>

// TO REMOVE! FOR TESTING ONLY
#include "Graphics.h"


std::shared_ptr<Input> INPUTMGR;

void Input::Initialize()
{
	/* Populate currstate with current key */
	//(void)GetKeyboardState(currstate);
	ReadKeyboard();
}

void Input::Update(float _dt)
{
	if (WINDOWS->outoffocus)
		return;
	UNREFERENCED_PARAMETER(_dt);
	/* Copy currstate to prevstate */
	for (size_t i = 0; i < KEYS_BUFFER_LENGTH; i++)
		prevstate[i] = currstate[i];
	/* Populate currstate array */
	//(void)GetKeyboardState(currstate);
	ReadKeyboard();

	// test 
	//std::cout << GetMousePosition().x << " | " << GetMousePosition().y << std::endl;
	//GRAPHICS->WorldToScreen(GRAPHICS->ScreenToWorld(GetMousePosition()));
}

void Input::Clear()
{
}

bool Input::GetCurrState(unsigned char key)
{
	return (currstate[key] & 0x8000);
}

bool Input::GetPrevState(unsigned char key)
{
	return (prevstate[key] & 0x8000);
}

void Input::ReadKeyboard()
{
	for (int x = 0; x < KEYS_BUFFER_LENGTH; x++)
		currstate[x] = static_cast<char>(GetAsyncKeyState(x) >> 8);
}

bool Input::GetKeyPressed(unsigned char key)
{
	// curr = 1, prev = 0
	return (!GetPrevState(key) && GetCurrState(key));
}

bool Input::GetKeyHold(unsigned char key)
{
	// curr = 1, prev = 1
	return GetCurrState(key);
}

bool Input::GetKeyRelease(unsigned char key)
{
	// curr = 0, prev = 1
	return (GetPrevState(key) && !GetCurrState(key));
}

VEC2 Input::GetMousePosition()
{
	//get the cursor position relative to screen coordinates
	(void)GetCursorPos(&mousePos);
	// map position to exe window screen 
	(void)ScreenToClient(WINDOWS->m_hWnd, &mousePos);
	// convert to VEC2 type & return
	VEC2 pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	// convert
	pos.x *= GRAPHICS->GetGraphicsWidth() / static_cast<float>(WinWidth);
	pos.y *= GRAPHICS->GetGraphicsHeight() / static_cast<float>(WinHeight);

	// flip
	pos.y = GRAPHICS->GetGraphicsHeight() - pos.y;

	return pos;
}

void Input::SetCursorView(bool value)
{
	ShowCursor(value); // TRUE shows cursor
}