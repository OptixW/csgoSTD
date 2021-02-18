#include "INITGlobal.hpp"
#include "IBaseGame.hpp"
#include <thread>

std::string process_name = "csgo.exe";
Memory mem(process_name);

void initialization();
void espThread();
visual g_Visual;
int init::client_dll;
int init::engine_dll;
int init::client_state;
constexpr int mod_cl_size = 0x5487000;

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

	std::string module_client = "client.dll";
	std::string module_engine = "engine.dll";
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

	std::shared_ptr<LocalPlayer> lp(new LocalPlayer);
	int game_state;
	while (true)
	{
		game_state = mem.RPM<DWORD>(init::client_state + signatures::dwClientState_State);
		if (game_state != IN_GAME)
		{
			Sleep(1000);
			continue;
		}

		lp->SetBase(mem.RPM<DWORD>(init::client_dll + signatures::dwLocalPlayer));

		g_Aimbot.update(lp, init::client_state);
		g_Aimbot.frame();
		g_Aimbot.TriggerBot(lp->getEntityByCrosshairID((lp->getCrosshairID())));
		Sleep(5);
	}
	Sleep(10000);
}

void espThread()
{
	std::shared_ptr<LocalPlayer> lp(new LocalPlayer);
	int game_state;
	while (true)
	{
		game_state = mem.RPM<DWORD>(init::client_state + signatures::dwClientState_State);
		if (game_state != IN_GAME)
		{
			Sleep(1000);
			continue;
		}
		lp->SetBase(mem.RPM<DWORD>(init::client_dll + signatures::dwLocalPlayer));
		g_Visual.update(lp);
		g_Visual.GlowEsp();
		Sleep(5);
	}
}