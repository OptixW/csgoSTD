#pragma once
#include "Math.hpp"
#include "Offsets.hpp"
#include "Memory.hpp"
#include "IBaseGame.hpp"
#include "Weapon.hpp"
class LocalPlayer {
public:
	int getWeaponId() const;
	bool Pistol() const;
	size_t getHP() const;
	size_t getTeam() const;
	size_t getCrosshairID() const;
	size_t getShotsFireID() const;
	Vector getPunchAngle() const;
	Vector getPos() const;
	DWORD getInput() const;
	Vector getEyeView() const; // only for localplayer
	std::shared_ptr<LocalPlayer> getEntityByCrosshairID(int crosshairID) const;
	size_t GetBase();
	DWORD getBoneObj() const;
	DWORD getMyGlowIndex() const;
	void SetBase(size_t);
	bool isAlive() const;

private:
	size_t base_;
};