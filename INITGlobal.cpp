#include "INITGlobal.hpp"
#include "IBaseGame.hpp"
#include <thread>
using namespace std::string_literals;
auto process_name = "csgo.exe"s;
Memory mem(process_name);

void initialization();
void espThread();
visual g_Visual;
int init::client_dll;
int init::engine_dll;
int init::client_state;
constexpr int mod_cl_size = 0x5488000;

void initialization() {
	mem.getProcessID();
	try {
		mem.openProcess();
	}
	catch (const char* ex) {
		std::cout << ex;
		Sleep(10000);
		exit(0);
	}

	auto module_client = "client.dll"s;
	auto module_engine = "engine.dll"s;
	if (mem.getModuleSize(module_client) != mod_cl_size)
	{
		std::cout << "The game was updated\n";
		Sleep(1000 * 5);
		exit(0);
	}

	init::client_dll = mem.getModuleBase(module_client);
	init::engine_dll = mem.getModuleBase(module_engine);
	init::client_state = mem.RPM<int>(init::engine_dll + signatures::dwClientState);

	CAimbot g_Aimbot;

	std::thread thr(espThread);
	thr.detach();
	


	LocalPlayer lp;
	LocalPlayer Entity;
	int game_state = 0;
	while (true)
	{
		game_state = mem.RPM<DWORD>(init::client_state + signatures::dwClientState_State);
		if (game_state != IN_GAME)
		{
			Sleep(1000);
			continue;
		}

		lp.SetBase(mem.RPM<DWORD>(init::client_dll + signatures::dwLocalPlayer));

		g_Aimbot.update(lp, init::client_state);
		g_Aimbot.frame();
		Entity.SetBase(lp.getEntityByCrosshairID((lp.getCrosshairID())));
		g_Aimbot.TriggerBot(Entity);
		Sleep(10);
	}
	Sleep(10000);
}

void espThread()
{
	LocalPlayer lp;
	int game_state;
	while (true)
	{
		game_state = mem.RPM<DWORD>(init::client_state + signatures::dwClientState_State);
		if (game_state != IN_GAME)
		{
			Sleep(1000);
			continue;
		}
		lp.SetBase(mem.RPM<DWORD>(init::client_dll + signatures::dwLocalPlayer));
		g_Visual.update(lp);
		g_Visual.GlowEsp();
		Sleep(5);
	}
}