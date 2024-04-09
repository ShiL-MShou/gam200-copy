/*************************************************************************************/
/*
File Name:		MyGraphicsFormat.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:		MyGraphicsFormat contains, VertexArray, VertexBuffer, IndexBuffer
			and Texture


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "MyGraphicsFormat.h"

#include "GraphicsVerticesType.h"

#include "Graphics.h"

#include "RapidjsonSerialization.h"

MyGraphicsFormat Create_MyGraphicsFormat(std::string _name, std::string _texturePath, JsonReader& _jr, const std::string& _key1)
{
	UNREFERENCED_PARAMETER(_name);

	std::shared_ptr<VertexArray>	vertexArrayPtr;
	std::shared_ptr<VertexBuffer>	vertexBufferPtr;
	std::shared_ptr<IndexBuffer>	indexBufferPtr;
	std::shared_ptr<Texture>		texturePtr;		// convert into unqiue ptr?


	texturePtr = GRAPHICS->LoadTexture(_texturePath);

	texturePtr->Serialize(_jr, _key1);

	GraphicsVerticesType positions = {
		-0.5f, -0.5f, 1.0f, 0.0f, 1.0f,	// 0
		 0.5f, -0.5f, 1.0f, /*texturePtr->GetTextureCoordsDiff().x*/ 1.0f, 1.0f,	// 1
		 0.5f,  0.5f, 1.0f, /*texturePtr->GetTextureCoordsDiff().x*/ 1.0f, 0.0f,	// 2
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f	// 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3 // 2, 3, 0
	};

	vertexArrayPtr = std::make_shared<VertexArray>();

	vertexBufferPtr = std::make_unique<VertexBuffer>(positions);


	// "add" vertexBuffer into vertexArray
	VertexBufferLayout layout;
	layout.Push<float>(3);		// vertex/ length
	layout.Push<float>(2);		// texture coords
	vertexArrayPtr->AddBuffer(*vertexBufferPtr, layout);

	indexBufferPtr = std::make_shared<IndexBuffer>(indices, 6);



	return std::make_tuple(_name, vertexArrayPtr, vertexBufferPtr, indexBufferPtr, texturePtr);

	//return std::make_tuple(nullptr, nullptr, nullptr, nullptr);
	assert(0);
}
