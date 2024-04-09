/*****************************************************************************/
/*
File Name:		TextSerialization.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "ISerialization.h"
#include <fstream>

class TextSerialization : public ISerialization
{
public:
	std::ifstream stream;
	virtual bool OpenFile(const std::string& filename) override;
	virtual bool IsGood() override;
	virtual void ReadFloat(float& f) override;
	virtual void ReadInt(int& i) override;
	virtual void ReadUnsignedInt(unsigned int& i) override;
	virtual void ReadString(std::string& s) override;
	virtual void ReadBool(bool& _b) override;
};

