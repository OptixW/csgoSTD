#pragma once
#include "Local.hpp"
#include "Math.hpp"
#include "IBaseGame.hpp"
#include <memory>
constexpr int bone_head = 8;
constexpr int bone_chest = 6;
class CAimbot
{
public:
	CAimbot();
	void ClampAngles(Vector& angles) const;
	void RCS();
	void calcAngle(Vector& source, Vector& dst, Vector& out) const;
	void update(LocalPlayer* pl, DWORD cl_state);
	void frame();
	void GetViewAngles(Vector& angles) const;
	void SetViewAngles(const Vector& angles) const;
	void TriggerBot(const LocalPlayer* Entity) const;
	void getBonePos(int boneID, const LocalPlayer* Entity, Vector& out) const;
private:
	DWORD cl_state_;
	LocalPlayer* lp_;
}; 
extern CAimbot g_Aimbot;


