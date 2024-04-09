/************************************************************************************ /
/*
File Name:		Renderer.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Renderer for graphics. Mostly not used anymore.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once


#include <glew.h>//from http://glew.sourceforge.net/

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// openGL error checking
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// debug/ error msg
void GLClearError();


bool GLLogCall(const char* fn, const char* file, int line);

class Renderer
{


public:
	void Clear() const;

	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const;
	void DrawDebug(std::shared_ptr<VertexArray> _vaPtr, std::shared_ptr<IndexBuffer> _ibPtr, std::shared_ptr<Shader> _sPtr) const;
	void DrawLine(std::shared_ptr<VertexArray> _vaPtr, std::shared_ptr<IndexBuffer> _ibPtr, std::shared_ptr<Shader> _sPtr) const;
};