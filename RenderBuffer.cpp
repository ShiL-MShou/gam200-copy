/*************************************************************************************/
/*
File Name:		RenderBuffer.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			RenderBuffer for graphics


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(unsigned int _width, unsigned int _height)
	: m_rendererID{ 0 }
{
	glGenRenderbuffers(1, &m_rendererID);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rendererID);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		assert(0);
	}

	Unbind();
}

RenderBuffer::~RenderBuffer()
{
}

RenderBuffer::RenderBuffer(const RenderBuffer& _rhs)
	:m_rendererID{ _rhs.m_rendererID }
{
}

RenderBuffer& RenderBuffer::operator=(const RenderBuffer& _rhs)
{
	m_rendererID = _rhs.m_rendererID;

	return *this;
}

void RenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_rendererID);
}

void RenderBuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
