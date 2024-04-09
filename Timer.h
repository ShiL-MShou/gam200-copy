/************************************************************************************ /
/*
File Name:		Timer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]

Brief:			Definition for Timer

Copyright Information:
All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once


class Timer
{
	bool isActive;
	float timeLeft;

public:
	Timer(bool _activeState);
	~Timer();
	Timer(const Timer& _rhs);
	Timer& operator=(const Timer& _rhs);


	void Update();
	
	bool IsTimeUp();

	bool IsActive();
	void SetIsActive(bool _state);

	float GetTimeLeft() { return timeLeft; }
	void SetTimeLeft(float _time) { timeLeft = _time; }
};

