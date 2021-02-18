#include "IniFiles.hpp"

IniFiles::IniFiles()
{
}
void IniFiles::WriteString(std::string str[3])
{
	TCHAR Dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Dir);
	WritePrivateProfileString(str[0].c_str(), str[1].c_str(), str[2].c_str(), _T("./config.ini"));
}
int IniFiles::ReadInt(std::string& section, std::string& key)
{
	int return_value = GetPrivateProfileInt(section.c_str(), key.c_str(), -1, _T("./config.ini"));

	return return_value;
}

IniFiles::~IniFiles()
{
}