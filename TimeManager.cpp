/************************************************************************************ /
/*
File Name:		TimerManager.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Manager for all the timers (NOT USED)


Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "TimeManager.h"


TimeManager::TimeManager()
	: dt{ 0 }, dtAvg{ 0 }, dtSum{ 0 }, counter{ 0 }
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::Initialize()
{
	dt		= 0;
	dtAvg	= 0;
	dtSum	= 0;
	counter = 0;
}

void TimeManager::Update(float _dt)
{
	dt = _dt;
	dtSum += dt;
	++counter;

	// Update average dt
	if (dtSum > DTUPDATEINTERVAL)
	{
		dtAvg = counter/ dtSum;

		// reset
		counter	= 0;
		dtSum	= 0;
	}

}

float TimeManager::GetFixedDt()
{
	return FIXEDDT;
}

float TimeManager::GetDt()
{
	return dt;
}

float TimeManager::GetAvgDt()
{
	return dtAvg;
}
