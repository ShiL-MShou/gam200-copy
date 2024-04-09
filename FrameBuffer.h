/*************************************************************************************/
/*
File Name:		FrameBuffer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			FrameBuffer for Graphics.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once


class FrameBuffer
{
	unsigned int m_rendererID;

public:
	FrameBuffer();
	~FrameBuffer();

	FrameBuffer(const FrameBuffer& _rhs);
	FrameBuffer& operator=(const FrameBuffer& _rhs);

	void Bind();
	void Unbind();
};