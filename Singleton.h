/*************************************************************************************/
/*
File Name:		Singleton.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

template <typename T>
class Singleton
{
	//static T mInstance;

protected:
	explicit Singleton<T>() {}
	virtual ~Singleton<T>() {}

public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
