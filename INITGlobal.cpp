#include "INITGlobal.hpp"
#include <thread>
std::string process_name = "csgo.exe";
Memory mem(process_name);
std::string module_client = "client.dll";
std::string module_engine = "engine.dll";

void initialization();


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

	int client_dll = mem.getModuleBase(module_client);
	int engine_dll = mem.getModuleBase(module_engine);
	int cl_state = mem.RPM<int>(engine_dll + signatures::dwClientState);
	CAimbot g_Aimbot;
	visual g_Visual;
	while (true)
	{
		auto game_state = mem.readInt(cl_state + signatures::dwClientState_State);
		if (game_state != IN_GAME)
		{
			Sleep(1000);
			continue;
		}

		LocalPlayer* lp = new LocalPlayer();
		lp->SetBase(mem.RPM<DWORD>(client_dll + signatures::dwLocalPlayer));
		
		g_Aimbot.update(lp, cl_state);
		g_Aimbot.frame();
		g_Aimbot.TriggerBot(getEntityByCrosshairID(lp->getCrosshairID(), client_dll));
		g_Visual.update(lp); //TODO
		g_Visual.GlowEsp();
		Sleep(10);

	}

	Sleep(10000);
}

LocalPlayer* getEntityByCrosshairID(int crosshairID, int client)
{
	LocalPlayer* pl = new LocalPlayer();
	pl->SetBase(mem.readInt(client + signatures::dwEntityList + (crosshairID - 1) * 0x10));
	return pl;
}
