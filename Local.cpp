#include "Local.hpp"



size_t LocalPlayer::getHP() const{
    ptrdiff_t offset = base_ + netvars::m_iHealth;
    return mem.readInt(offset);
}

size_t LocalPlayer::getTeam() const {
    ptrdiff_t offset = base_ + netvars::m_iTeamNum;
    return mem.readInt(offset);
}

size_t LocalPlayer::getCrosshairID() const {
    ptrdiff_t offset = base_ + netvars::m_iCrosshairId;
    return mem.readInt(offset);
}

size_t LocalPlayer::getShotsFireID() const
{   
    ptrdiff_t offset = base_ + netvars::m_iShotsFired;
    return mem.readInt(offset);
}




Vector2 LocalPlayer::getPunchAngle() const
{
    ptrdiff_t offset = base_ + netvars::m_aimPunchAngle;
    return mem.RPM<Vector2>(offset);
}






void LocalPlayer::SetBase(size_t b) {
    base_ = b;
}

bool LocalPlayer::isAlive() {
    return mem.RPM<bool>(base_ + netvars::m_lifeState);
}
