#pragma once
#include "Local.hpp"
#include "Math.hpp"
class CAimbot
{
public:
	CAimbot();
	void ClampAngles(Vector2& angles);
	void ClampAngles(Vector3& angles);
	void RCS();
	void update(LocalPlayer* pl, DWORD cl_state);
	void frame();
	void GetViewAngles(Vector2& angles);
	void SetViewAngles(const Vector2& angles)const;
	void TriggerBot(const LocalPlayer* Entity) const;
private:
	DWORD cl_state_;
	LocalPlayer* lp_;
}; 
extern CAimbot g_Aimbot;


