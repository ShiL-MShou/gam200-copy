/*************************************************************************************/
/*
File Name:		RenderBuffer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			RenderBuffer for graphics


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

class RenderBuffer
{
	unsigned int m_rendererID;

public:
	RenderBuffer(unsigned int _width, unsigned int _height);
	~RenderBuffer();

	RenderBuffer(const RenderBuffer& _rhs);
	RenderBuffer& operator=(const RenderBuffer& _rhs);

	void Bind();
	void Unbind();
};