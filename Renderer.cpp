/************************************************************************************ /
/*
File Name:		Renderer.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Renderer for graphics. Mostly not used anymore.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "Renderer.h"
#include <iostream>

// debug/ error msg
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* fn, const char* file, int line)
{
	while (GLenum err = glGetError())
	{
		std::cout << "[Error] " << err << " "
			<< fn << " " << file
			<< " : " << line << std::endl;

		return false;
	}

	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	s.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawDebug(std::shared_ptr<VertexArray> _vaPtr, std::shared_ptr<IndexBuffer> _ibPtr, std::shared_ptr<Shader> _sPtr) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	_sPtr->Bind();
	_vaPtr->Bind();
	_ibPtr->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, _ibPtr->GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawLine(std::shared_ptr<VertexArray> _vaPtr, std::shared_ptr<IndexBuffer> _ibPtr, std::shared_ptr<Shader> _sPtr) const
{
	_sPtr->Bind();
	_vaPtr->Bind();
	//_ibPtr->Bind();

	glDrawArrays(GL_LINES, 0, 2);
}
