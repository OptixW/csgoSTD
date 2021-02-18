#pragma once
#include <Windows.h>
#include <string>
#include <tchar.h>
#include <map>
#include <vector>
class IniFiles
{
public:

	static void WriteString(std::string str[3]);
	static int ReadInt(std::string& section, std::string& key);
	IniFiles();
	~IniFiles();
	struct cheat_params
	{
		static std::vector<std::string> field_name_;
		struct params
		{
			static std::vector<std::string> section;
			static std::vector<std::string> value;
		};
	};
};