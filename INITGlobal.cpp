#include "INITGlobal.hpp"
std::string process_name = "csgo.exe";
Memory mem(process_name);



inline void ClampAngles(Vector2& angles)
{
	if (angles.y < -180.0f)
		angles.y += 360.0f;
	if (angles.y > 180.0f)
		angles.y -= 360.0f;
	if (angles.x < -89.0f)
		angles.x = -89.0f;
	if (angles.x > 89.0f)
		angles.x = 89.0f;
}

[[noreturn]] void initialization() {

	std::string module_client = "client.dll";
	std::string module_engine = "engine.dll";

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

	LocalPlayer* lp = new LocalPlayer();
	

	std::shared_ptr<LocalPlayer> Entity(new LocalPlayer());
	Vector2 angle;
	Vector2 old;
	
	while (true)
	{
		auto cl_state = mem.RPM<DWORD>(engine_dll + signatures::dwClientState);
		
			lp->SetBase(mem.RPM<DWORD>(client_dll + signatures::dwLocalPlayer));
			if (lp->getShotsFireID() > 1 && GetAsyncKeyState(0x01))
			{
				Vector2 m_PunchAngle = lp->getPunchAngle();
                auto mView = mem.RPM<Vector2>(cl_state + signatures::dwClientState_ViewAngles);
				mView += old;
				m_PunchAngle *= 2.0;
				angle = mView - m_PunchAngle;	
				ClampAngles(angle);
				mem.WPM<Vector2>(cl_state + signatures::dwClientState_ViewAngles, angle);
				Sleep(10);
				old = m_PunchAngle;
				}
			else {
				old.zero();
			}
		
		if (GetAsyncKeyState(VK_MENU) != 0) {
			Entity->SetBase(mem.readInt(client_dll + signatures::dwEntityList + (lp->getCrosshairID() - 1) * 0x10));

			if (Entity->getHP() > 0 && Entity->getTeam() != lp->getTeam()) {
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				Sleep(5);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
			
			Sleep(10);

		}
	}

	Sleep(10000);
}


