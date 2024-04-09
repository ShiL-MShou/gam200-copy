/************************************************************************************ /
/*
File Name:		IndexBuffer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Index buffer to reuse the vertices.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

class IndexBuffer
{
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	IndexBuffer();

	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
