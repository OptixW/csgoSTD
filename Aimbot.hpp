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
using smart_loc = std::shared_ptr<LocalPlayer>;
class CAimbot
{
public:

	CAimbot();
	void ClampAngles(Vector& angles) const;
	void RCS();
	void calcAngle(Vector& source, Vector& dst, Vector& out) const;
	void update(smart_loc& Entity, DWORD cl_state);
	void frame();
	void getBestTarget();
	int nearestBone(const smart_loc& Entity) const;
	float distnt(Vector EntityPos, Vector MyPos) const;
	void GetViewAngles(Vector& angles) const;
	void SetViewAngles(const Vector& angles) const;
	void SilentSetViewAngles(const Vector& angles) const;
	void TriggerBot(const smart_loc& Entity) const;
	void getBonePos(int boneID, const smart_loc& Entity, Vector& out) const;
	float GetFOV(const Vector& viewangles, const Vector& vSrc, const Vector& vEnd) const;
	float DynamicFov(smart_loc Entity) const;
	void smoothAngle(Vector& currentAngle, float fSmoothPercentage, Vector& angles) const;
	void MakeVector(const Vector& vIn, Vector& vOut) const;
	void VelocityCompansate(Vector& EntPos);
	void Shoot() const;
private:
	float fov_;
	int BestIndex_;
	DWORD cl_state_;
	smart_loc lp_;
};
extern CAimbot g_Aimbot;