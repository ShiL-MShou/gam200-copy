/*************************************************************************************/
/*
File Name:		MyGraphicsFormat.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:		MyGraphicsFormat contains, VertexArray, VertexBuffer, IndexBuffer
			and Texture


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include <tuple>
#include <memory>	// smart pointers

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

//unsigned int myGraphicsFormatCounter = 1;

typedef std::tuple<
	std::string,
	std::shared_ptr<VertexArray>,
	std::shared_ptr<VertexBuffer>,
	std::shared_ptr<IndexBuffer>,
	std::shared_ptr<Texture>
	//std::shared_ptr<Shader>
>  MyGraphicsFormat;


MyGraphicsFormat Create_MyGraphicsFormat(std::string _name, std::string _texturePath, JsonReader& _jr, const std::string& _key1);
 

