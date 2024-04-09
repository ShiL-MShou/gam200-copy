/*************************************************************************************/
/*
File Name:		FrameBuffer.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			FrameBuffer for Graphics.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
	: m_rendererID{ 0 }
{
	glGenFramebuffers(1, &m_rendererID);

	Bind();
}

FrameBuffer::~FrameBuffer()
{

}

FrameBuffer::FrameBuffer(const FrameBuffer& _rhs)
	: m_rendererID{ _rhs.m_rendererID }
{
}

FrameBuffer& FrameBuffer::operator=(const FrameBuffer& _rhs)
{
	m_rendererID = _rhs.m_rendererID;

	return *this;
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
