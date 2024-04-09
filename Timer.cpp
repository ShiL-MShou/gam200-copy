/************************************************************************************ /
/*
File Name:		Timer.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Definition for Timer

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Timer.h"

#include "TimeManager.h"


Timer::Timer(bool _activeState)
	: timeLeft{ 0 }, isActive{ _activeState }
{
}

Timer::~Timer()
{
}

Timer::Timer(const Timer& _rhs)
	: timeLeft{ _rhs.timeLeft } , isActive{ _rhs.isActive }
{
}

Timer& Timer::operator=(const Timer& _rhs)
{
	timeLeft = _rhs.timeLeft;

	return *this;
}

void Timer::Update()
{
	if (isActive)
		timeLeft -= MyTimeManager.GetDt();
}

bool Timer::IsTimeUp()
{
	return timeLeft <= 0.0f;
}

bool Timer::IsActive()
{
	return isActive;
}

void Timer::SetIsActive(bool _state)
{
	isActive = _state;
}
