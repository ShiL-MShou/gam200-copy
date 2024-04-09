/************************************************************************************ /
/*
File Name:		Frame.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This holds all the necessary information for a frame.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <array>
#include "Vector2.h"


struct Frame
{
	std::array<VEC2, 4> uv;
	float timeDelay;

	Frame();
	Frame(std::array<VEC2, 4> _arr);
	~Frame();
};

