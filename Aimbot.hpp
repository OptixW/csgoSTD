#pragma once
#include "Local.hpp"

#include "Math.hpp"
#include "IBaseGame.hpp"
#include <algorithm>
#include <memory>

namespace BoneEnum {
	enum bone_t
	{
		bone_head = 8,
		bone_neck = 7,
		bone_chest = 6
		//7 - NECK ; 6 CHEST
	};
	static const bone_t bones[] = { bone_head, bone_neck, bone_chest };
}
class CAimbot
{
public:

	CAimbot();
	void ClampAngles(Vector& angles) const;
	void RCS();
	void calcAngle(Vector& source, Vector& dst, Vector& out) const;
	void update(const LocalPlayer& pl, const DWORD cl_state);
	void frame()
	{
		//RCS();

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (!lp_.isJump())
			{
				mem.WPM<int>(init::client_dll + signatures::dwForceJump, 5);
			}
			else
			{
				mem.WPM<int>(init::client_dll + signatures::dwForceJump, 4);
			}
		}

		if (GetAsyncKeyState(0x01) & 0x8000)
		{
			{
				if (lp_.scopeWeapon())
				{
					return;
				}

				Vector myView;
				Vector out;
				Vector out2;
				getBestTarget();
				Entity_.SetBase(mem.RPM<int>(init::client_dll + signatures::dwEntityList + BestIndex_ * 0x10));
				if (DynamicFov(Entity_) < 4.0)
				{
					auto MyPos = lp_.getPos() + lp_.getEyeView();
					auto EntPos = Entity_.getPos();
					getBonePos(nearestBone(Entity_), Entity_, out2);
					calcAngle(MyPos, out2, out2);
					out2 -= lp_.getPunchAngle() * 2.0f;
					ClampAngles(out2);
					GetViewAngles(myView);
					if (Entity_.getHP() > 0)
					{
						smoothAngle(myView, 30, out2);
						SetViewAngles(out2);
					}
					else
					{
						BestIndex_ = -1;
						return;
					}
				}
				else
					return;


				Sleep(10);
			}
		}
	}
	float AngleDifference(const Vector& ViewAngles, const Vector& TargetAngles, float Distance) const;
	
	void getBestTarget();
	int nearestBone(const LocalPlayer& Entity) const;
	float distnt(Vector EntityPos, Vector MyPos) const;
	void GetViewAngles(Vector& angles) const;
	void SetViewAngles(const Vector& angles) const;
	void SilentSetViewAngles(const Vector& angles) const;
	void TriggerBot(const LocalPlayer& Entity) const;
	void getBonePos(int boneID, const LocalPlayer& Entity, Vector& out) const;
	float GetFOV(const Vector& viewangles, const Vector& vSrc, const Vector& vEnd) const;
	float DynamicFov(const LocalPlayer& Entity) const;
	void smoothAngle(Vector& currentAngle, float fSmoothPercentage, Vector& angles) const;
	void MakeVector(const Vector& vIn, Vector& vOut) const;
	void VelocityCompansate(Vector& EntPos);
	void Shoot() const;
private:
	float fov_;
	int BestIndex_;
	DWORD cl_state_;
	LocalPlayer lp_;
	LocalPlayer Entity_;
};
extern CAimbot g_Aimbot;