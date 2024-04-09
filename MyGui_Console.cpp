#include "stdafx.h"

#include "MyGui_Console.h"
#include "MyGuiManager.h"

void MyGui_Console::Open()
{
}

void MyGui_Console::Update()
{
	if (MyGuiManager::show_console) LoadConsole(&MyGuiManager::show_console);
}

void MyGui_Console::Exit()
{
}

void MyGui_Console::LoadConsole(bool* _open)
{

}