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
	void frame();
	
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