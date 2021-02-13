#include "IniFiles.hpp"

IniFiles::IniFiles()
{
}
int IniFiles::ReadInt(char* section, char* key)
{
	return GetPrivateProfileInt(section, key, -1, _T("./config.ini"));
}
void IniFiles::WriteString(char* section, char* key, char* value)
{
	TCHAR Dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Dir);
	WritePrivateProfileString(section, key, value, _T("./config.ini"));
}

IniFiles::~IniFiles()
{
}