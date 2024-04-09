/*****************************************************************************/
/*
File Name:		TextSerialization.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "TextSerialization.h"

bool TextSerialization::OpenFile(const std::string& filename)
{
	stream.open(filename);
	return stream.is_open();
}

bool TextSerialization::IsGood()
{
	return stream.good();
}

void TextSerialization::ReadFloat(float& f)
{
	stream >> f;
}

void TextSerialization::ReadInt(int& i)
{
	stream >> i;
}

void TextSerialization::ReadUnsignedInt(unsigned int& i)
{
	stream >> i;
}

void TextSerialization::ReadString(std::string& s)
{
	stream >> s;
}

void TextSerialization::ReadBool(bool& _b)
{
	stream >> _b;
}
