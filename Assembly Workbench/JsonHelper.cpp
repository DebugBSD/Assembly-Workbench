#include "stdafx.h"
#include "JsonHelper.h"

bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* pInProperty, int& outInt)
{
	auto itr = inObject.FindMember(pInProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsInt())
	{
		return false;
	}

	outInt = property.GetInt();
	return true;
}

bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsDouble())
	{
		return false;
	}

	outFloat = property.GetDouble();
	return true;
}

bool JsonHelper::GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsString())
	{
		return false;
	}

	outStr = property.GetString();
	return true;
}

bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsBool())
	{
		return false;
	}

	outBool = property.GetBool();
	return true;
}

void JsonHelper::AddInt(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, int value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddFloat(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, float value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddString(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, const std::string& value)
{
	rapidjson::Value v;
	v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()),
		alloc);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddBool(rapidjson::Document::AllocatorType& alloc,
	rapidjson::Value& inObject, const char* name, bool value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
