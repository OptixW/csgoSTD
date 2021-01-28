#include "Aimbot.hpp"



void CAimbot::ClampAngles(Vector2& angles)
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

void CAimbot::ClampAngles(Vector3& angles)
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






void CAimbot::calcAngle(const Vector& source, const Vector& dst, Vector &out)
{
	Vector delta;
	
	delta = source - dst;
	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	out.x = (float)(atan(delta.z / hyp) * 57.295779513082f);
	out.y = (float)(atan(delta.y / delta.x) * 57.295779513082f);
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
	static Vector2 old;
	Vector2 mView;
	Vector2 angle;
	if (lp_->getShotsFireID() > 1 && GetAsyncKeyState(0x01))
	{
		Vector2 m_PunchAngle = lp_->getPunchAngle();
		GetViewAngles(mView);
		mView += old;
		m_PunchAngle *= 2.0;
		angle = mView - m_PunchAngle;
		ClampAngles(angle);
		SetViewAngles(angle);
		Sleep(10);
		old = m_PunchAngle;
	}
	else {
		old.zero();
	}
}

void CAimbot::update(LocalPlayer* pl, DWORD cl_state)
{
	if (pl == nullptr)
		return;
	lp_ = pl;
	cl_state_ = cl_state;

}

void CAimbot::GetViewAngles(Vector2& angles)
{
	angles = mem.RPM<Vector2>(cl_state_ + signatures::dwClientState_ViewAngles);
}

void CAimbot::SetViewAngles(const Vector2& angles) const
{
	mem.WPM<Vector2>(cl_state_ + signatures::dwClientState_ViewAngles, angles);
}

CAimbot::CAimbot()
{
}

void CAimbot::TriggerBot(const LocalPlayer * Entity) const
{
	if (GetAsyncKeyState(VK_MENU) != 0) {
		if (Entity->getHP() > 0 && Entity->getTeam() != lp_->getTeam()) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(10);
	}
}


