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
#pragma once

#include <vector>
#include <array>

#include "Vector2.h"
#include "Vector3.h"

#include "Transform.h"

#include "glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"

#include "Singleton.h"

#define CIRCLEVERTICESNUM 32
#define CONEVERTICESNUM 8
#define CONERADIUS 1
#define CONEANGLE 20
#define MyDebugDraw (DebugDraw::GetInstance())


class DebugDraw : public Singleton<DebugDraw>
{
	VEC3 m_RGB;

	float zBuffer;

	float lines[6];
	float rect[12];
	float circle[CIRCLEVERTICESNUM * 3];
	float cone[CONEVERTICESNUM * 3];

	Renderer renderer;

	std::shared_ptr<VertexArray>	vertexArrayPtr;
	std::unique_ptr<VertexBuffer>	vertexBufferPtr;

	std::shared_ptr<VertexArray>	vertexArrayLinePtr;
	std::unique_ptr<VertexBuffer>	vertexBufferLinePtr;

	std::shared_ptr<VertexArray>	vertexArrayCirclePtr;
	std::unique_ptr<VertexBuffer>	vertexBufferCirclePtr;

	std::shared_ptr<VertexArray>	vertexArrayConePtr;
	std::unique_ptr<VertexBuffer>	vertexBufferConePtr;

	std::shared_ptr<VertexArray>	vertexArrayDebugPtr;
	std::unique_ptr<VertexBuffer>	vertexBufferDebugPtr;

	std::vector<glm::mat4> debugLineList;
	std::vector<glm::mat4> debugRectList;
	std::vector<glm::mat4> debugCircleList;
	std::vector<glm::mat4> debugConeList;
	std::vector<glm::mat4> debugDebugList;

	glm::mat4 debugSelectedobj;

	std::vector<std::array<float, 16>> debugTestList;

	DebugDraw();
	~DebugDraw();

	DebugDraw(const DebugDraw&) = delete;
	DebugDraw& operator=(const DebugDraw&) = delete;

	friend class Singleton<DebugDraw>;
	friend class Graphics;

public:
	//static DebugDraw Instance;

	void Initalize();
	void DrawLine(VEC2 _startVec, VEC2 _endVec);
	void DrawRect(VEC2 _position, VEC2 _size);
	void DrawCircle(VEC2 _position, float _radius);
	void DrawCone(VEC2 _position, float _scale, float _rotation);

	void DrawTest(VEC2 _startVec, VEC2 _endVec);

	void DrawDebug(VEC2 _startVec, VEC2 _endVec);

	void DrawSelected(VEC2 _position, VEC2 _size);

	// setters and getters
	void SetColor(VEC3 _RGB);

	void Clear();

	void DrawAll(std::shared_ptr<Shader> _shaderPtr, glm::mat4 _vp);
};

