#include "Aimbot.hpp"
#include <iostream>




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





void CAimbot::calcAngle(Vector& source, Vector& dst, Vector &out) const
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

void CAimbot::frame()
{
	RCS();
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
		m_PunchAngle *= 1.7f;
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

void CAimbot::update(LocalPlayer* pl, DWORD cl_state)
{
	if (pl == nullptr)
		return;
	lp_ = pl;
	cl_state_ = cl_state;

}

int CAimbot::nearestBone(const std::shared_ptr<LocalPlayer> Entity) const
{
	return 8;
}


void CAimbot::getBonePos(int boneID, const std::shared_ptr<LocalPlayer> Entity, Vector &out) const
{
	auto boneBase = Entity->getBoneObj();
	Vector vBone;
	vBone.x = mem.RPM<float>(boneBase + 0x30 * boneID + 0x0C);
	vBone.y = mem.RPM<float>(boneBase + 0x30 * boneID + 0x1C);
	vBone.z = mem.RPM<float>(boneBase + 0x30 * boneID + 0x2C);
	out = vBone;
}

void CAimbot::GetViewAngles(Vector& angles) const
{

	angles = mem.RPM<Vector>(init::client_state + signatures::dwClientState_ViewAngles);
}

void CAimbot::SetViewAngles(const Vector& angles) const
{
	mem.WPM<float>(init::client_state +signatures::dwClientState_ViewAngles, angles.x);
	mem.WPM<float>(init::client_state + signatures::dwClientState_ViewAngles+0x4, angles.y);
}

CAimbot::CAimbot()
{
}

void CAimbot::TriggerBot(const std::shared_ptr<LocalPlayer> Entity) const
{
	if (GetAsyncKeyState(VK_MENU) != 0) {
		if (Entity->getHP() > 0 && Entity->getTeam() != lp_->getTeam()) {
			[&]() {
				Vector source = lp_->getPos() + lp_->getEyeView();
				Vector target;
				getBonePos(bone_head, Entity, target);
				calcAngle(source, target, target);
				target -= lp_->getPunchAngle() * 2.0f;
				SetViewAngles(target);
				
			}();
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(10);
	}
}


