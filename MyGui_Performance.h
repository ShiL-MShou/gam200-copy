/*************************************************************************************/
/*
File Name:		MyGui_Performance.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "MyGui.h"

class MyGui_Performance :public MyGui
{
	bool show_performance_data;

	void LoadPerformanceData(bool* _open);
public:
	MyGui_Performance() : show_performance_data{ true }
	{}

	void Open() override;

	void Update() override;

	void Exit() override;
};

