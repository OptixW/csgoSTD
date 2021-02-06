#include "Memory.hpp"

void Memory::getProcessID()
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 proc_info_;
	proc_info_.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &proc_info_))
	{
		if (process_name_ == proc_info_.szExeFile) {
			process_id_ = proc_info_.th32ProcessID;
			CloseHandle(snapshot);
			return;
		}
	}
	while (Process32Next(snapshot, &proc_info_))
	{
		if (process_name_ == proc_info_.szExeFile) {
			process_id_ = proc_info_.th32ProcessID;
			CloseHandle(snapshot);
			return;
		}
	}

	process_id_ = -1;
}

void Memory::openProcess() {
	if (process_id_ == -1) {
		throw "process not found!";
	}
	else if (process_id_ == -2) {
		throw "call the 'getProcessId' with arg before using openProcess=)";
	}

	process_handle_ = OpenProcess(PROCESS_ALL_ACCESS, false, process_id_);
	if (process_handle_ == NULL)
		throw "cannot open this process";
}

int Memory::getModuleBase(std::string& module_name) const {
	MODULEENTRY32 mod;

	auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id_);
	mod.dwSize = sizeof(mod);
	if (Module32First(snap, &mod))
	{
		if (module_name == mod.szModule)
		{
			CloseHandle(snap);
			return (int)mod.modBaseAddr;
		}
	}
	while (Module32Next(snap, &mod))
	{
		if (module_name == mod.szModule)
		{
			CloseHandle(snap);
			return int(mod.modBaseAddr);
		}
	}
	return -1;
}

int Memory::getModuleSize(std::string& module_name) const
{
	MODULEENTRY32 mod;

	auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id_);
	mod.dwSize = sizeof(mod);
	if (Module32First(snap, &mod))
	{
		if (module_name == mod.szModule)
		{
			CloseHandle(snap);
			return (int)mod.modBaseSize;
		}
	}
	while (Module32Next(snap, &mod))
	{
		if (module_name == mod.szModule)
		{
			CloseHandle(snap);
			return int(mod.modBaseSize);
		}
	}
	return -1;
}

int Memory::readInt(int address) const {
	return mem.RPM<int>(address);
}

float Memory::readFloat(int address) const {
	return mem.RPM<float>(address);
}

double Memory::readDouble(int address) const {
	return mem.RPM<double>(address);
}