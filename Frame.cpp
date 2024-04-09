/************************************************************************************ /
/*
File Name:		Frame.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This holds all the necessary information for a frame.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Frame.h"

#define DEFAULTTIMEDELAY 0.5f

Frame::Frame()
	: uv{}, timeDelay{ DEFAULTTIMEDELAY }
{
}

Frame::Frame(std::array<VEC2, 4> _arr)
	: timeDelay{ DEFAULTTIMEDELAY }
{
	std::copy(std::begin(_arr), std::end(_arr), std::begin(uv));
}

Frame::~Frame()
{
}
