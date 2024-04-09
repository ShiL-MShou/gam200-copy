/************************************************************************************ /
/*
File Name:		TimerManager.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Manager for all the timers (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once
#include "Singleton.h"

#define FIXEDDT 1/60.0f
#define DTUPDATEINTERVAL 1/6.0f
#define MyTimeManager (TimeManager::GetInstance())


class TimeManager : public Singleton<TimeManager>
{
	float dt;
	float dtAvg;

	float dtSum;
	int counter;

	TimeManager();
	~TimeManager();

	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;

	friend class Singleton<TimeManager>;

public:
	void Initialize();
	void Update(float _dt);


	float GetFixedDt();
	float GetDt();
	float GetAvgDt();
};

