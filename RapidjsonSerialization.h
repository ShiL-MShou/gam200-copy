/*****************************************************************************/
/*
File Name:		RapidjsonSerialization.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

Copyright information:
All content copyright 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once
#include "JsonHeaders.h"

#include <fstream>
#include <iostream>


class JsonWriter
{

public:
	rapidjson::StringBuffer stringBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriterBuffer;

	JsonWriter() : prettyWriterBuffer{ stringBuffer }
	{}

	void CreateKey(const std::string& _key)
	{
		prettyWriterBuffer.Key(_key.c_str());
	}

	void CreateStringMember(const std::string& _key, const std::string& _string)
	{
		prettyWriterBuffer.Key(_key.c_str());
		prettyWriterBuffer.String(_string.c_str());
	}

	void CreateFloatMember(const std::string& _key, const float& _f)
	{
		prettyWriterBuffer.Key(_key.c_str());
		prettyWriterBuffer.Double(static_cast<double>(_f));
	}

	void CreateIntMember(const std::string& _key, const int& _i)
	{
		prettyWriterBuffer.Key(_key.c_str());
		prettyWriterBuffer.Int(_i);
	}

	void WriteToJsonFile(const std::string& _filepath)
	{
		std::ofstream of(_filepath);
		of << stringBuffer.GetString();
	}
};


class JsonReader
{
public:
	const std::string filepath;
	std::ifstream stream;
	rapidjson::IStreamWrapper streamWrapper;
	rapidjson::Document doc;
	rapidjson::StringBuffer stringBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyReaderBuffer;



	//1) make a JReader obj with a filename
	JsonReader(const std::string& _filepath) :
		filepath{ _filepath },
		stream(filepath),
		streamWrapper{ stream },
		prettyReaderBuffer{ stringBuffer }
	{

	}



	//-----------------UNUSED--------------------
	//2) call this to open the file
	//bool OpenJsonFile()
	//{
	//	stream.open( filepath );
	//	return stream.is_open();
	//}

	bool IsFileOpened() const
	{
		return stream.is_open();
	}

	//use to check if good
	bool IsGood()
	{
		return stream.good();
	}

	//-----------------UNUSED-----------------------
	//3) wrap the ifstream member "stream" in a json wrapper
	//rapidjson::IStreamWrapper& WrapStream()
	//{
	//	rapidjson::IStreamWrapper isw( stream );
	//	return isw;
	//}

	//4) parse the wrapped stream to the rapidjson::Document
	void ParseStreamToDoc()
	{
		doc.ParseStream(streamWrapper);
	}

	//=============================================================
	void ReadIntMember(const std::string& _key1, const std::string& _key2, int& _i)
	{
		if (doc.HasMember(_key1.c_str()))
		{
			if (doc[_key1.c_str()].HasMember(_key2.c_str()))
			{
				const rapidjson::Value& variable = doc[_key1.c_str()][_key2.c_str()];
				_i = variable.GetInt();
			}
		}

	}

	void ReadFloatMember(const std::string& _key1, const std::string& _key2, float& _f)
	{
		if (doc.HasMember(_key1.c_str()))
		{
			if (doc[_key1.c_str()].HasMember(_key2.c_str()))
			{
				const rapidjson::Value& variable = doc[_key1.c_str()][_key2.c_str()];
				_f = variable.GetFloat();
			}
		}
	}

	void ReadBoolMember(const std::string& _key1, const std::string& _key2, bool& _b)
	{
		if (doc.HasMember(_key1.c_str()))
		{
			if (doc[_key1.c_str()].HasMember(_key2.c_str()))
			{
				const rapidjson::Value& variable = doc[_key1.c_str()][_key2.c_str()];
				_b = variable.GetBool();
			}
		}
	}

	void ReadStringMember(const std::string& _key1, const std::string& _key2, std::string& _s)
	{
		if (doc.HasMember(_key1.c_str()))
		{
			if (doc[_key1.c_str()].HasMember(_key2.c_str()))
			{
				const rapidjson::Value& variable = doc[_key1.c_str()][_key2.c_str()];
				_s = variable.GetString();
			}
		}
	}

	void ReadStringMember(const std::string& _key1, const std::string& _key2, const std::string& _key3, std::string& _s)
	{
		if (doc.HasMember(_key1.c_str()))
		{
			if (doc[_key1.c_str()].HasMember(_key2.c_str()))
			{
				if (doc[_key1.c_str()][_key2.c_str()].HasMember(_key3.c_str()))
				{
					const rapidjson::Value& variable = doc[_key1.c_str()][_key2.c_str()][_key3.c_str()];
					_s = variable.GetString();
				}
			}
		}
	}

	void ReadIntMember(const std::string& _key1, const std::string& _key2, const std::string& _key3, int& _i)
	{
		if (doc.HasMember(_key1.c_str()))
		{
			if (doc[_key1.c_str()].HasMember(_key2.c_str()))
			{
				if (doc[_key1.c_str()][_key2.c_str()].HasMember(_key3.c_str()))
				{
					const rapidjson::Value& variable = doc[_key1.c_str()][_key2.c_str()][_key3.c_str()];
					_i = variable.GetInt();
				}
			}
		}
	}

	void ReadStringArrayMember(const std::string& _key, std::vector<std::string>& _vec)
	{
		if (doc.HasMember(_key.c_str()))
		{
			//  reading for array
			const rapidjson::Value& tmpArr = doc[_key.c_str()];

			// looping through the array
			for (rapidjson::SizeType i = 0; i < tmpArr.Size(); i++) // Uses SizeType instead of size_t
			{
				//_vec.push_back(static_cast<T>(tmpArr[i]));
				if (tmpArr[i].IsString())
					_vec.push_back(tmpArr[i].GetString());


				// safety checks (?)
				//if (testArr[i].IsString())
				{
					//printf(testArr[i].GetString());
					
				}
					
				//printf("\n");
			}
		}

		for (auto& mem : _vec)
		{
			std::cout << mem << std::endl;
		}
	}
};

template <typename T>
inline void StreamRead(JsonReader& _stream, T& type)
{
	type.Serialize(_stream);
}

template <typename T>
inline void StreamWrite(JsonWriter& _stream, T& type)
{
	type.DeSerialize(_stream);
}


