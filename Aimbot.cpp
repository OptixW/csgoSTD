#include "Aimbot.hpp"
#include <iostream>
#include <vector>
void CAimbot::ClampAngles(Vector& angles) const
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

void CAimbot::calcAngle(Vector& source, Vector& dst, Vector& out) const
{
	Vector delta;

	delta = source - dst;
	auto hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);

	out.x = RAD2DEG((atan(delta.z / hyp)));
	out.y = RAD2DEG((atan(delta.y / delta.x)));
	out.z = 0.0f;

	if (delta.x >= 0.0)
		out.y += 180.0f;
}
void CAimbot::VelocityCompansate(Vector& EntPos)
{
	float comp = 0.13f;
	EntPos += mem.readFloat(lp_.GetBase() + netvars::m_vecVelocity) * comp;
}

void CAimbot::Shoot() const
{
	mem.WPM<int>(init::client_dll + signatures::dwForceAttack, 5);
	Sleep(40);
	mem.WPM<int>(init::client_dll + signatures::dwForceAttack, 4);
}

void CAimbot::getBestTarget()
{
	float max_fov = 9999.0f;
	Vector EntityPos;
	float temp_fov = 0;
	Vector myViewAngles;

	Vector source;
	Vector target;
	for (int i = 1; i < 32; ++i)
	{
		Entity_.SetBase(mem.RPM<int>(init::client_dll + signatures::dwEntityList + i * 0x10));

		if (!Entity_.GetBase())
			continue;
		if (Entity_.getHP() > 0 && Entity_.getTeam() != lp_.getTeam() && !Entity_.isDormant())
		{
			GetViewAngles(myViewAngles);
			source = lp_.getPos() + lp_.getEyeView();
			getBonePos(nearestBone(Entity_), Entity_, EntityPos);

			temp_fov = GetFOV(myViewAngles, source, EntityPos);
			if (temp_fov < max_fov)
			{
				max_fov = temp_fov;
				BestIndex_ = i;
			}
		}
	}
}

float CAimbot::distnt(Vector EntityPos, Vector MyPos) const
{
	return sqrt(pow((EntityPos.x - MyPos.x), 2) + pow((EntityPos.y - MyPos.y), 2) + pow((EntityPos.z - MyPos.z), 2));
}

void CAimbot::RCS()//todo
{
	static Vector old;
	static Vector start_;
	static bool spray_ = false;
	static Vector mView;
	Vector angle;
	Vector m_PunchAngle;
	if (GetAsyncKeyState(0x01) & 0x8000)
	{
		if (lp_.getShotsFireID() > 1)
		{
			Vector m_PunchAngle = lp_.getPunchAngle();
			GetViewAngles(mView);
			start_ = mView;
			mView += old;

			start_ = mView + m_PunchAngle;
			m_PunchAngle *= 2.0f;
			angle = mView - m_PunchAngle;
			start_ = mView + m_PunchAngle;
			ClampAngles(angle);
			SetViewAngles(angle);
			old = m_PunchAngle;
		}
		else
		{
			//	SetViewAngles(start_);
			old.Zero();
		}
	}
}

void CAimbot::update(const LocalPlayer& pl, const DWORD cl_state)
{
	lp_ = pl;
	cl_state_ = cl_state;
}

void CAimbot::frame()
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
						if (!lp_.scopeWeapon())
						{
							smoothAngle(myView, 30, out2);
						}
						if(lp_.iClip() > 0)
						SetViewAngles(out2);
					}
					else
					{
						BestIndex_ = -1;
						return;
					}
				}
			

				Sleep(5);
			}
		}
	}

void CAimbot::SilentSetViewAngles(const Vector& angles) const // not working without CreateMove Hooking
{
	int current_sequence_number = mem.RPM<int>(init::client_state + signatures::clientstate_last_outgoing_command);
	std::cout << current_sequence_number << std::endl;

	DWORD dwUserCMD = mem.RPM<DWORD>(lp_.getInput() + 0x1);
	dwUserCMD += (current_sequence_number % 150) * 0x64;
	int iUserCMDSequenceNumberBase = 0;
	iUserCMDSequenceNumberBase = mem.RPM<int>(dwUserCMD + 0x0);
	int iUserCMDSequenceNumber = 0;

	//	mem.WPM<byte>(init::engine_dll + signatures::dwbSendPackets, 0);
	while (iUserCMDSequenceNumber != current_sequence_number)
	{
		iUserCMDSequenceNumber = mem.readInt(iUserCMDSequenceNumberBase + 0x4);
	}
	//
	// your command to pcmd
	//
	//mem.WPM<byte>(init::engine_dll + signatures::dwbSendPackets, 1);
	//mem.WPM<Vector>(dwUserCMD + 0xC, angles);write your viewAngles
}

void CAimbot::getBonePos(int boneID, const LocalPlayer &Entity, Vector& out) const
{
	auto boneBase = Entity.getBoneObj();
	Vector vBone;
	vBone.x = mem.RPM<float>(boneBase + 0x30 * boneID + 0x0C);
	vBone.y = mem.RPM<float>(boneBase + 0x30 * boneID + 0x1C);
	vBone.z = mem.RPM<float>(boneBase + 0x30 * boneID + 0x2C);
	out = vBone;
}

float CAimbot::GetFOV(const Vector& viewangles, const Vector& vSrc, const Vector& vEnd) const
{
	Vector vAng, vAim, vAngles;
	vAngles = viewangles;
	auto vSrc_ = vSrc;
	auto vEnd_ = vEnd;
	calcAngle(vSrc_, vEnd_, vAng);
	MakeVector(vAngles, vAim);
	MakeVector(vAng, vAng);

	return RAD2DEG(acos(vAim.Dot(vAng)) / vAim.LengthSqr());
}

void CAimbot::smoothAngle(Vector& currentAngle, float fSmoothPercentage, Vector& angles) const
{
	float smoothing = fSmoothPercentage;
	Vector viewangles = currentAngle;

	Vector delta = angles - viewangles;

	float target_pitch = angles.y;
	float view_pitch = viewangles.y;
	float smooth_factor = 100.f;

	if (angles.y < 0) target_pitch = 360.f + angles.y;
	if (viewangles.y < 0) view_pitch = 360.f + viewangles.y;

	float pitch = min(abs(target_pitch - view_pitch), 360.f - abs(target_pitch - view_pitch));

	if (viewangles.y > 90.f && angles.y < -90.f)
	{
		angles.x = viewangles.x + delta.x / smooth_factor * smoothing;
		angles.y = viewangles.y + pitch / smooth_factor * smoothing;
		if (angles.y > 180.f)
			angles.y = -360.f + angles.y;
		return;
	}
	if (viewangles.y < -90.f && angles.y > 90.f)
	{
		angles.x = viewangles.x + delta.x / smooth_factor * smoothing;
		angles.y = viewangles.y - pitch / smooth_factor * smoothing;
		if (angles.y < -180.f)
			angles.y = 360.f + angles.y;
		return;
	}

	angles.x = viewangles.x + delta.x / smooth_factor * smoothing;
	angles.y = viewangles.y + delta.y / smooth_factor * smoothing;
	angles.z = 0.0f;
	ClampAngles(angles);
}

float CAimbot::DynamicFov(const LocalPlayer& Entity) const 
{
	float fPlayerDistance, fYawDegreeDifference;
	Vector MyView = {};
	GetViewAngles(MyView);
	Vector vTargetAngles = {};
	Vector myPos = lp_.getPos() + lp_.getEyeView();
	Vector EntityPos = {};
	getBonePos(7, Entity, EntityPos);
	calcAngle(myPos, EntityPos, vTargetAngles);
	fPlayerDistance = EntityPos.Dot(myPos);
	auto AngleDifference_ = AngleDifference(MyView, vTargetAngles, fPlayerDistance);
	auto result = RAD2DEG(atan(AngleDifference_ / myPos.Dot(EntityPos)));
	return result;
}

float CAimbot::AngleDifference(const Vector& ViewAngles, const Vector& TargetAngles, float Distance) const
{
	auto pitch = sinf(DEG2RAD(fabs(ViewAngles.x - TargetAngles.x))) * Distance;
	auto yaw = sinf(DEG2RAD(fabs(ViewAngles.y - TargetAngles.y))) * Distance;

	return sqrt(powf(pitch, 2.0) + powf(yaw, 2.0));
}

void CAimbot::MakeVector(const Vector& vIn, Vector& vOut) const
{
	auto pitch = DEG2RAD(vIn.x);
	auto yaw = DEG2RAD(vIn.y);
	auto temp = cos(pitch);
	vOut.x = -temp * -cos(yaw);
	vOut.y = sin(yaw) * temp;
	vOut.z = -sin(pitch);
}

void CAimbot::GetViewAngles(Vector& angles) const
{
	angles = mem.RPM<Vector>(init::client_state + signatures::dwClientState_ViewAngles);
}

int CAimbot::nearestBone(const LocalPlayer& Entity) const
{
	Vector pos = lp_.getPos() + lp_.getEyeView();
	Vector out;
	Vector Entity_pos = Entity.getPos();
	float delta_ = 9999.0f;
	Vector ViewAngles;
	Vector res;
	int bone_res = 8;
	GetViewAngles(ViewAngles);
	for (auto i = 0; i < sizeof(BoneEnum::bones); i++)
	{
		getBonePos(BoneEnum::bones[i], Entity, out);
		calcAngle(pos, out, out);

		if (AngleDifference(ViewAngles, out, out.Length()) < delta_)
		{
			delta_ = AngleDifference(ViewAngles, out, out.Length());
			bone_res = BoneEnum::bones[i];
		}
	}
	return bone_res;
}

void CAimbot::SetViewAngles(const Vector& angles) const
{
	mem.WPM<float>(init::client_state + signatures::dwClientState_ViewAngles, angles.x);
	mem.WPM<float>(init::client_state + signatures::dwClientState_ViewAngles + 0x4, angles.y);
}

CAimbot::CAimbot()
{
}
void CAimbot::TriggerBot(const LocalPlayer& Entity) const
{
	if (GetAsyncKeyState(VK_MENU) != 0) {
		if (Entity.getHP() > 0 && Entity.getTeam() != lp_.getTeam()) {
			Shoot();
			
		}
		Sleep(5);
	}
}