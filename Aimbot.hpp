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
		bone_chest = 6,
		bone_neck = 7
	};
	static const bone_t bones[] = { bone_head, bone_chest, bone_neck };
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
	void getBestTarget(smart_loc& s_Entity) const;//todo
	int nearestBone(const smart_loc& Entity) const;
	float distnt(Vector EntityPos, Vector MyPos) const;
	void GetViewAngles(Vector& angles) const;
	void SetViewAngles(const Vector& angles) const;
	void SilentSetViewAngles(const Vector& angles) const;
	void TriggerBot(const smart_loc& Entity) const;
	void getBonePos(int boneID, const smart_loc& Entity, Vector& out) const;
private:
	DWORD cl_state_;
	smart_loc lp_;
};
extern CAimbot g_Aimbot;