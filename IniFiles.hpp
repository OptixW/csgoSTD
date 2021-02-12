#pragma once
#include <Windows.h>
#include <tchar.h>
class IniFiles
{
public:

	static void WriteString(char * section, char * key, char * value);
	static int ReadInt(char * section, char * key);
	IniFiles();
	~IniFiles();
};


