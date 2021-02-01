#include "Local.hpp"
#include <iostream>


int LocalPlayer::getWeaponId() const
{
    int weapon = mem.readInt(base_ + netvars::m_hActiveWeapon);
    int weaponEnt = mem.readInt(init::client_dll + signatures::dwEntityList + ((weapon & 0xFFF) - 1) * 0x10); 
    int weaponID = mem.readInt(weaponEnt + netvars::m_iItemDefinitionIndex);
    return weaponID;
}

bool LocalPlayer::Pistol() const
{
    return IsPistol(getWeaponId());
}

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


std::shared_ptr<LocalPlayer> LocalPlayer::getEntityByCrosshairID(int crosshairID) const
{
    std::shared_ptr<LocalPlayer> pl(new LocalPlayer());
    pl->SetBase(mem.readInt(init::client_dll + signatures::dwEntityList + (crosshairID - 1) * 0x10));
    return pl;
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
