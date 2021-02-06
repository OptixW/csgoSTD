#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <memory>
class Memory {
public:
	Memory(const std::string& proc_name) :process_name_(proc_name) {
		process_id_ = -2;
	}
	~Memory()
	{
		if (process_handle_ != nullptr)
			CloseHandle(process_handle_);
	}

	void getProcessID();
	void openProcess();
	int getModuleBase(std::string& module_name) const;
	int getModuleSize(std::string& module_name) const;
	int readInt(int address) const;
	float readFloat(int address) const;
	double readDouble(int address) const;

	template<typename T>
	void WPM(int address, T data)
	{
		SIZE_T out;
		WriteProcessMemory(process_handle_, (LPVOID)address, &data, sizeof(data), &out);
	}

	template <typename T>
	T RPM(int address) {
		T object;
		SIZE_T out;
		ReadProcessMemory(process_handle_, (LPCVOID)address, &object, sizeof(object), &out);
		return object;
	}

private:
	Memory(const Memory&) = delete;
	void operator=(const Memory&) = delete;
	int process_id_;
	HANDLE process_handle_;
	std::string process_name_;
}; extern Memory mem;