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




Vector LocalPlayer::getPunchAngle() const
{
    ptrdiff_t offset = base_ + netvars::m_aimPunchAngle;
    return mem.RPM<Vector>(offset);
}
Vector LocalPlayer::getPos() const
{
    return mem.RPM<Vector>(base_ + netvars::m_vecOrigin);
}

Vector LocalPlayer::getEyeView() const
{
    return mem.RPM<Vector>(base_ + netvars::m_vecViewOffset);
}






size_t LocalPlayer::GetBase()
{
    return base_;
}

DWORD LocalPlayer::getBoneObj() const
{
    return mem.RPM<DWORD>(base_ + netvars::m_dwBoneMatrix);
}

DWORD LocalPlayer::getMyGlowIndex() const
{
    return mem.RPM<DWORD>(base_ + netvars::m_iGlowIndex);
}

void LocalPlayer::SetBase(size_t b) {
    base_ = b;
}

bool LocalPlayer::isAlive() const 
{
    return mem.RPM<bool>(base_ + netvars::m_lifeState);
}
