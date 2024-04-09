/************************************************************************************ /
/*
File Name:		DebugDraw.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			This handles and manages all the debug drawings.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "DebugDraw.h"
#include "CoreEngine.h"

#include "Graphics.h" // for #Define

#include "BoxCollider.h" // for AABB

#define PI           3.14159265358979323846f  /* pi */

//DebugDraw DebugDraw::Instance;

DebugDraw::DebugDraw()
{
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::Initalize()
{
	zBuffer = -1.9f;
	
	// --- line ---
	float tmp[6] = {
		0.0f, 0.0f, zBuffer,
		1.0f, 0.0f, zBuffer
	};

	std::swap(lines, tmp);

	// --- rect ---
	float tmp2[12] = {
		-0.5f, -0.5f, zBuffer,	// 0
		 0.5f, -0.5f, zBuffer,	// 1
		 0.5f,  0.5f, zBuffer,	// 2
		-0.5f,  0.5f, zBuffer	// 3
	};

	std::swap(rect, tmp2);

	// --- circle ---
	float inc = 360.0f / CIRCLEVERTICESNUM * PI / 180.0f;
	
	for (int i = 0; i < CIRCLEVERTICESNUM; ++i)
	{
		circle[i*3]		= cosf(i * inc);
		circle[i*3 + 1] = sinf(i * inc);
		circle[i*3 + 2] = zBuffer;
	}

	// --- cone ---
	inc = (float)CONEANGLE / ((float)CONEVERTICESNUM - 1);

	cone[0] = 0.0f;
	cone[1] = 0.0f;
	cone[2] = zBuffer;

	for (int i = 1; i < CONEVERTICESNUM; ++i)
	{
		cone[i * 3]		= cosf((CONEANGLE/2.0f - ((i-1) * inc))* PI / 180.0f);
		cone[i * 3 + 1] = sinf((CONEANGLE/2.0f - ((i-1) * inc))* PI / 180.0f);
		cone[i * 3 + 2] = zBuffer;
	}

	vertexArrayPtr = std::make_shared<VertexArray>();
	vertexBufferPtr = std::make_unique<VertexBuffer>((void*)rect, (unsigned int)(3 * 4 * sizeof(float)));

	vertexArrayLinePtr = std::make_shared<VertexArray>();
	vertexBufferLinePtr = std::make_unique<VertexBuffer>((void*)lines, (unsigned int)(3 * 2 * sizeof(float)));

	vertexArrayCirclePtr = std::make_shared<VertexArray>();
	vertexBufferCirclePtr = std::make_unique<VertexBuffer>((void*)circle, (unsigned int)(3 * CIRCLEVERTICESNUM * sizeof(float)));

	vertexArrayConePtr = std::make_shared<VertexArray>();
	vertexBufferConePtr = std::make_unique<VertexBuffer>((void*)cone, (unsigned int)(3 * CONEVERTICESNUM * sizeof(float)));

	vertexArrayDebugPtr = std::make_shared<VertexArray>();
	vertexBufferDebugPtr = std::make_unique<VertexBuffer>((void*)lines, (unsigned int)(3 * 2 * sizeof(float)));

	// "add" vertexBuffer into vertexArray
	VertexBufferLayout layout;
	layout.Push<float>(3);		// vertex/ length
	
	vertexArrayLinePtr->AddBuffer(*vertexBufferLinePtr, layout);
	vertexArrayPtr->AddBuffer(*vertexBufferPtr, layout);
	vertexArrayCirclePtr->AddBuffer(*vertexBufferCirclePtr, layout);
	vertexArrayConePtr->AddBuffer(*vertexBufferConePtr, layout);

	vertexArrayDebugPtr->AddBuffer(*vertexBufferDebugPtr, layout);
}

void DebugDraw::DrawLine(VEC2 _startVec, VEC2 _endVec)
{
	std::shared_ptr<Camera> tmpCamera = GRAPHICS->GetMainCameraPtr();

	AABB camAABB(VEC2(-tmpCamera->GetPosition().x, -tmpCamera->GetPosition().y),
		VEC2(-tmpCamera->GetPosition().x + tmpCamera->GetScreenWidth(), -tmpCamera->GetPosition().y + tmpCamera->GetScreenHeight()));


	AABB rectAABB(_startVec, _endVec);

	if (camAABB.CheckCollided(&rectAABB))
	{
		float dist = sqrt((_startVec.x - _endVec.x) * (_startVec.x - _endVec.x) + (_startVec.y - _endVec.y) * (_startVec.y - _endVec.y));

		Transform tmpTrans;
		tmpTrans.SetPosition(_startVec);
		tmpTrans.SetScale({ dist, 1});

		float angle = atan2(_endVec.y - _startVec.y, _endVec.x - _startVec.x);
		tmpTrans.SetRotation(angle * 180.0f / PI);

		debugLineList.push_back(tmpTrans.ConvertIntoMat4());
	}
}

void DebugDraw::DrawRect(VEC2 _position, VEC2 _size)
{
	std::shared_ptr<Camera> tmpCamera = GRAPHICS->GetMainCameraPtr();

	AABB camAABB(	VEC2(-tmpCamera->GetPosition().x, -tmpCamera->GetPosition().y),
					VEC2(-tmpCamera->GetPosition().x + tmpCamera->GetScreenWidth(), -tmpCamera->GetPosition().y + tmpCamera->GetScreenHeight()));


	AABB rectAABB(	VEC2(_position.x - _size.x, _position.y - _size.y),
					VEC2(_position.x + _size.x, _position.y + _size.y));

	if (camAABB.CheckCollided(&rectAABB))
	{
		Transform tmpTrans;
		tmpTrans.SetPosition(_position);
		tmpTrans.SetScale(_size);
	
		debugRectList.push_back(tmpTrans.ConvertIntoMat4());
	}
}

void DebugDraw::DrawCircle(VEC2 _position, float _radius)
{
	Transform tmpTrans;
	tmpTrans.SetPosition(_position);
	tmpTrans.SetScale(_radius, _radius);

	debugCircleList.push_back(tmpTrans.ConvertIntoMat4());
}

void DebugDraw::DrawCone(VEC2 _position, float _scale, float _rotation)
{
	Transform tmpTrans;
	tmpTrans.SetPosition(_position);
	tmpTrans.SetScale(_scale, _scale);
	tmpTrans.SetRotation(_rotation);

	debugConeList.push_back(tmpTrans.ConvertIntoMat4()); 
}

void DebugDraw::DrawTest(VEC2 _startVec, VEC2 _endVec)
{
	float dist = sqrt((_startVec.x - _endVec.x) * (_startVec.x - _endVec.x) + (_startVec.y - _endVec.y) * (_startVec.y - _endVec.y));

	Transform tmpTrans;
	tmpTrans.SetPosition(_startVec);
	tmpTrans.SetScale({ dist, 1 });

	float angle = atan2(_endVec.y - _startVec.y, _endVec.x - _startVec.x);
	tmpTrans.SetRotation(angle * 180.0f / PI);

	debugTestList.push_back(tmpTrans.ConvertTest());
}

void DebugDraw::DrawDebug(VEC2 _startVec, VEC2 _endVec)
{
	float dist = sqrt((_startVec.x - _endVec.x) * (_startVec.x - _endVec.x) + (_startVec.y - _endVec.y) * (_startVec.y - _endVec.y));

	Transform tmpTrans;
	tmpTrans.SetPosition(_startVec);
	tmpTrans.SetScale({ dist, 1 });

	float angle = atan2(_endVec.y - _startVec.y, _endVec.x - _startVec.x);
	tmpTrans.SetRotation(angle * 180.0f / PI);

	debugDebugList.push_back(tmpTrans.ConvertIntoMat4());
}

void DebugDraw::DrawSelected(VEC2 _position, VEC2 _size)
{
	Transform tmpTrans;
	tmpTrans.SetPosition(_position);
	tmpTrans.SetScale(_size);

	debugSelectedobj = tmpTrans.ConvertIntoMat4();
}

void DebugDraw::SetColor(VEC3 _RGB)
{
	m_RGB = _RGB;
}

void DebugDraw::Clear()
{
	debugRectList.clear();
	debugLineList.clear();
	debugCircleList.clear();
	debugConeList.clear();

	debugTestList.clear();

	debugDebugList.clear();
}

void DebugDraw::DrawAll(std::shared_ptr<Shader> _shaderPtr, glm::mat4 _vp)
{
	_shaderPtr->Bind();

	int num = 0;

	int limitCounter = 0;

	// --- draw rect ---
	// green
	_shaderPtr->SetUniform3f("u_Color", 0.0f, 1.0f, 0.0f);
	vertexArrayPtr->Bind();

	for (glm::mat4 model : debugRectList)
	{
		_shaderPtr->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), _vp * model);

		++num;

		++limitCounter;

		if (limitCounter >= INSTANCELIMIT)
		{
			glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, num);
			limitCounter = 0;
			num = 0;
		}

	}
	glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, num);

	num = 0;
	limitCounter = 0;

	if (CORE->GetEditorStatus())
	{
		// draw selected obj
		// Yellow
		_shaderPtr->SetUniform3f("u_Color", 1.0f, 1.0f, 0.0f);
		vertexArrayPtr->Bind();

		_shaderPtr->SetUniformMat4f("u_MVPArr[0]", _vp * debugSelectedobj);

		glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, 1);
	}

	// --- draw line ---
	// red
	_shaderPtr->SetUniform3f("u_Color", 1.0f, 0.0f, 0.0f);
	vertexArrayLinePtr->Bind();

	for (glm::mat4 model : debugLineList)
	{
		_shaderPtr->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), _vp * model);
		
		++num;

		++limitCounter;

		if (limitCounter >= INSTANCELIMIT)
		{
			glDrawArraysInstanced(GL_LINES, 0, 2, num);
			limitCounter = 0;
			num = 0;
		}
	}
	glDrawArraysInstanced(GL_LINES, 0, 2, num);

	num = 0;
	limitCounter = 0;

	// --- draw circle ---
	// blue
	_shaderPtr->SetUniform3f("u_Color", 0.0f, 0.0f, 1.0f);
	vertexArrayCirclePtr->Bind();

	for (glm::mat4 model : debugCircleList)
	{
		_shaderPtr->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), _vp * model);
		
		++num;

		++limitCounter;

		if (limitCounter >= INSTANCELIMIT)
		{
			glDrawArraysInstanced(GL_LINE_LOOP, 0, CIRCLEVERTICESNUM, num);
			limitCounter = 0;
			num = 0;
		}
	}
	glDrawArraysInstanced(GL_LINE_LOOP, 0, CIRCLEVERTICESNUM, num);

	num = 0;
	limitCounter = 0;

	// --- draw cone ---
	// purple
	_shaderPtr->SetUniform3f("u_Color", 1.0f, 0.0f, 1.0f);

	vertexArrayConePtr->Bind();

	for (glm::mat4 model : debugConeList)
	{
		_shaderPtr->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), _vp * model);
		
		++num;

		++limitCounter;

		if (limitCounter >= INSTANCELIMIT)
		{
			glDrawArraysInstanced(GL_LINE_LOOP, 0, CONEVERTICESNUM, num);
			limitCounter = 0;
			num = 0;
		}
	}
	glDrawArraysInstanced(GL_LINE_LOOP, 0, CONEVERTICESNUM, num);

	num = 0;
	limitCounter = 0;

	//-----------------------------------------------------------------------------------------------------
	// red & G
	_shaderPtr->SetUniform3f("u_Color", 1.0f, 1.0f, 0.0f);
	vertexArrayLinePtr->Bind();

	for (glm::mat4 model : debugDebugList)
	{
		_shaderPtr->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), _vp * model);

		++num;

		++limitCounter;

		if (limitCounter >= INSTANCELIMIT)
		{
			glDrawArraysInstanced(GL_LINES, 0, 2, num);
			limitCounter = 0;
			num = 0;
		}
	}
	glDrawArraysInstanced(GL_LINES, 0, 2, num);

	


	//// --- test ---
	//
	//num = 0;

	//// red
	//_shaderPtr->SetUniform3f("u_Color", 1.0f, 0.0f, 0.0f);
	//vertexArrayLinePtr->Bind();

	//for (std::array<float, 16> model : debugTestList)
	//{
	//	//float test[16];
	//	glm::mat4 mat = glm::make_mat4(&model[0]);

	//	mat = _vp * mat;

	//	std::array<float, 16> tmpArr;


	//	const float* pSource = (const float*)glm::value_ptr(mat);
	//	for (int i = 0; i < 16; ++i)
	//		tmpArr[i] = pSource[i];


	//	_shaderPtr->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), tmpArr);// _vp* mat);
	//	++num;
	//}
	//glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, num);

	Clear();
}
