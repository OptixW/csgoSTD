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

class CAimbot
{
public:
	CAimbot();
	void ClampAngles(Vector& angles) const;
	void RCS();
	void calcAngle(Vector& source, Vector& dst, Vector& out) const;
	void update(std::shared_ptr<LocalPlayer>& Entity, DWORD cl_state);
	void frame();
	std::shared_ptr<LocalPlayer> getBestTarget() const;//todo
	int nearestBone(const std::shared_ptr < LocalPlayer>& Entity) const;
	void GetViewAngles(Vector& angles) const;
	void SetViewAngles(const Vector& angles) const;
	void SilentSetViewAngles(const Vector& angles) const;
	void TriggerBot(const std::shared_ptr<LocalPlayer>& Entity) const;
	void getBonePos(int boneID, const std::shared_ptr<LocalPlayer>& Entity, Vector& out) const;
private:
	DWORD cl_state_;
	std::shared_ptr<LocalPlayer> lp_;
};
extern CAimbot g_Aimbot;