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
	EntPos += mem.readFloat(lp_->GetBase() + netvars::m_vecVelocity) * comp;
}

void CAimbot::frame()
{
	RCS();

	/*while (GetAsyncKeyState(20) != 0)
	{
		smart_loc Entity(new LocalPlayer());

		Vector myView;
		Vector out;
		Vector out2;
		float fov = 5;
		getBestTarget();
		Entity->SetBase(mem.RPM<int>(init::client_dll + signatures::dwEntityList + BestIndex_ * 0x10));
		//while (Entity->getHP() > 0) {
			//{

			DynamicFov(Entity);
				auto MyPos = lp_->getPos() + lp_->getEyeView();
				auto EntPos = Entity->getPos();
				getBonePos(nearestBone(Entity), Entity, out2);
				calcAngle(MyPos, out2, out2);
				Vector Eye = lp_->getEyeView();
				out2 -= lp_->getPunchAngle() * 2.0f;
				ClampAngles(out2);
				
				GetViewAngles(myView);
				
				fov = GetFOV(myView, MyPos, EntPos);
				
				//std::cout << fov << std::endl;
				if (fov > 10)
				{
					return;
				}
				smoothAngle(myView, 10, out2);
				//SetViewAngles(out2);
				//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				Sleep(10);
				//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				//if (GetAsyncKeyState(0x18) != 0)
					//break;
			//}
		}
		*/
	}

void CAimbot::getBestTarget()
{
	float max_fov = 9999.0f;
	Vector EntityPos;
	float temp_fov = 0;
	Vector myViewAngles;
	smart_loc Entity;
	Vector source;
	Vector target;
	for (int i = 1; i < 32; ++i)
	{
		smart_loc Entity(new LocalPlayer());
		Entity->SetBase(mem.RPM<int>(init::client_dll + signatures::dwEntityList + i * 0x10));

		if (!Entity->GetBase())
			continue;
		if (Entity->getHP() > 0 && Entity->getTeam() != lp_->getTeam())
		{
			GetViewAngles(myViewAngles);
			source = lp_->getPos() + lp_->getEyeView();
			getBonePos(nearestBone(Entity), Entity, EntityPos);
			//calcAngle(source, target, target);
			
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
	Vector mView;
	Vector angle;
	if (lp_->getShotsFireID() > 1 && GetAsyncKeyState(0x01))
	{
		Vector m_PunchAngle = lp_->getPunchAngle();
		GetViewAngles(mView);
		mView += old;
		m_PunchAngle *= 1.8f;
		angle = mView - m_PunchAngle;
		ClampAngles(angle);
		SetViewAngles(angle);
		Sleep(10);
		old = m_PunchAngle;
	}
	else {
		old.Zero();
	}
}

void CAimbot::update(smart_loc& pl, DWORD cl_state)
{
	if (pl == nullptr)
		return;
	lp_ = pl;
	cl_state_ = cl_state;
}

void CAimbot::SilentSetViewAngles(const Vector& angles) const // not working without CreateMove Hooking
{
	int current_sequence_number = mem.RPM<int>(init::client_state + signatures::clientstate_last_outgoing_command);
	std::cout << current_sequence_number << std::endl;

	DWORD dwUserCMD = mem.RPM<DWORD>(lp_->getInput() + 0x1);
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

void CAimbot::getBonePos(int boneID, const smart_loc& Entity, Vector& out) const
{
	auto boneBase = Entity->getBoneObj();
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
		float smooth_factor = 120.f;

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

float CAimbot::DynamicFov(smart_loc Entity) const //todo
{

	float fRealDistance, fPlayerDistance, fYawDegreeDifference;
		

		Vector MyView;
		GetViewAngles(MyView);
		Vector vTargetAngles;
		Vector myPos = lp_->getPos();
		Vector EntityPos = lp_->getPos();
		calcAngle(myPos, EntityPos, vTargetAngles);
		fYawDegreeDifference = sin(MyView.x - vTargetAngles.x);
		fYawDegreeDifference = fabs(fYawDegreeDifference);
		std::cout << fYawDegreeDifference << std::endl;

		fPlayerDistance = myPos.DistToSqr(EntityPos);
	return 0.0f;
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

int CAimbot::nearestBone(const smart_loc& Entity) const
{
	Vector pos = lp_->getPos() + lp_->getEyeView();
	Vector out;
	Vector Entity_pos = Entity->getPos();
	float delta_ = 9999.0f;
	Vector ViewAngles;
	Vector res;
	int bone_res = 8;
	GetViewAngles(ViewAngles);
	for (auto i = 0; i < sizeof(BoneEnum::bones); i++)
	{
		getBonePos(BoneEnum::bones[i], Entity, out);
		calcAngle(pos, out, out);
		res = ViewAngles - out;
		ClampAngles(res);

		if (res.Length2DSqr() < delta_)
		{
			delta_ = res.Length2DSqr();
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

void CAimbot::TriggerBot(const smart_loc& Entity) const
{
	if (GetAsyncKeyState(VK_MENU) != 0) {
		if (Entity->getHP() > 0 && Entity->getTeam() != lp_->getTeam()) {
			[&](bool p) {
				if (!p)
					return;
				Vector myView;
				GetViewAngles(myView);
				Vector source = lp_->getPos() + lp_->getEyeView();
				Vector target;
				Vector smoothed_angle;
				getBonePos(nearestBone(Entity), Entity, target);
				calcAngle(source, target, target);
				target -= lp_->getPunchAngle() * 1.7f;
				smoothAngle(myView, 15, target);
				SetViewAngles(target);
			}(lp_->Pistol());
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(1);
	}
}