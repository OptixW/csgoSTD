#include "INITGlobal.hpp"
#include "IBaseGame.hpp"

std::string process_name = "csgo.exe";
Memory mem(process_name);

void initialization();


int init::client_dll;
int init::engine_dll;
int init::client_state;
void initialization(){
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

		init::client_dll = mem.getModuleBase(module_client);
		init::engine_dll = mem.getModuleBase(module_engine);
		init::client_state = mem.RPM<int>(init::engine_dll + signatures::dwClientState);

		
		CAimbot g_Aimbot;
		visual g_Visual;

		while (true)
		{
			auto game_state = mem.RPM<DWORD>(init::client_state +signatures::dwClientState_State);
			if (game_state != IN_GAME)
			{
				Sleep(1000);
				continue;
			}
			
			LocalPlayer* lp = new LocalPlayer();
			lp->SetBase(mem.RPM<DWORD>(init::client_dll + signatures::dwLocalPlayer));
			g_Aimbot.update(lp, init::client_state);
			g_Aimbot.frame();
			g_Aimbot.TriggerBot(lp->getEntityByCrosshairID((lp->getCrosshairID())));
			g_Visual.update(lp);
			g_Visual.GlowEsp();
			Sleep(5);

		}
		Sleep(10000);
	}
		




