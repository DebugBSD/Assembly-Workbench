#pragma once

#include "rapidjson/document.h"

#include <string>
#include <vector>

class JsonHelper
{
public:
	static bool GetInt(const rapidjson::Value& inObject, const char* pInProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
	static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);

	// Setter functions
	static void AddInt(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, int value);
	static void AddFloat(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, float value);
	static void AddString(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const std::string& value);
	static void AddBool(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, bool value);

};


