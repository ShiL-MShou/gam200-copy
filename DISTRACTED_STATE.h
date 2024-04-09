/******************************************************************************/
/*
	File Name: DISTRACTED_STATE.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Derived state for teacher NPC

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "State.h"
#include "List_StateLinker.h"

/* Connected to patrol */

class DISTRACTED_STATE : public iState
{
	VEC2 Destination;
	std::vector<VEC2> path_to_des;
	int _currPt;
	int _nextPt;
	VEC2 _UnitPos;
	float _pausetime;// = 15.0f;
	float _currtime;// = 0.0f;
	float speed;// = 100.0f;
	bool soundPlayed;

public:
	DISTRACTED_STATE();
	~DISTRACTED_STATE();

	void Update(float _dt) override;

	void OnEnter() override;

	void OnExit() override;

	void ResetState() override;

	void UpdateAnimation() override;

	void AddPathPoint(VEC2 v);

	void SetPathtoDes(std::vector<VEC2> list);

	void SetCurrPoint();

	void NextPoint();

	void MoveTo(float step);

};