/*****************************************************************************/
/*
File Name:		ISerialization.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "Utility.h"

class ISerialization
{
public:
	virtual bool OpenFile(const std::string& filename)=0;
	virtual bool IsGood()=0;
	virtual void ReadFloat(float& f) = 0;
	virtual void ReadInt(int& i) = 0;
	virtual void ReadUnsignedInt(unsigned int& i) = 0;
	virtual void ReadString(std::string &s) = 0;
	virtual void ReadBool(bool& _b) = 0;
};

template <typename T>
inline void StreamRead(ISerialization& _stream, T& type)	
{
	type.Serialize(_stream);
}

inline void StreamRead(ISerialization& _stream, float& f)
{
	_stream.ReadFloat(f);
}

inline void StreamRead(ISerialization& _stream, int& i)
{
	_stream.ReadInt(i);
}

inline void StreamRead(ISerialization& _stream, unsigned int& i)
{
	_stream.ReadUnsignedInt(i);
}

inline void StreamRead(ISerialization& _stream, std::string& s)
{
	_stream.ReadString(s);
}

inline void StreamRead(ISerialization& _stream, bool& _b)
{
	std::string b;
	_stream.ReadString(b);

	if (b == "true")
		_b = true;
	else
		_b = false;
}