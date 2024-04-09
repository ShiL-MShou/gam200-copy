/******************************************************************************/
/*
	File Name: IDLE_STATE.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Derived state for Teacher NPC

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "State.h"

/* Connected to patrol and distracted */
  
class IDLE_STATE : public iState
{
	VEC2 Distract_des;

public:
	IDLE_STATE();
	~IDLE_STATE();
	

	void OnEnter() override;

	void Update(float _dt) override;

	void UpdateAnimation() override;

	/* Getter & Setter for Distract_des */
	void SetDistractDes(VEC2 t);

	VEC2 GetDistractDes();
};