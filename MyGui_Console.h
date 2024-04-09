#pragma once
#include "MyGui.h"

class MyGui_Console : public MyGui
{
	void LoadConsole(bool* _open);

public:

	void Open() override;

	void Update() override;

	void Exit() override;
};

