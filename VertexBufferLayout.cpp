/************************************************************************************ /
/*
File Name:		VertexBufferLayout.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This tells the graphics the layout of the VertexBuffer.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"				// for ASSERT


unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:			return 4;
	case GL_UNSIGNED_INT:	return 4;
	case GL_UNSIGNED_BYTE:	return 1;
	}

	ASSERT(false);
	return 0;
}

VertexBufferLayout::VertexBufferLayout()
	: m_stride{ 0 }
{
}

VertexBufferLayout::~VertexBufferLayout()
{
}
