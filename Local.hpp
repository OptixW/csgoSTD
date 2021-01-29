#pragma once
#include "Math.hpp"
#include "Offsets.hpp"
#include "Memory.hpp"
class LocalPlayer {
public:

	size_t getHP() const;
	size_t getTeam() const;
	size_t getCrosshairID() const;
	size_t getShotsFireID() const;
	Vector getPunchAngle() const;
	Vector getPos() const;
	Vector getEyeView() const; // only for localplayer
	size_t GetBase();
	DWORD getBoneObj() const;
	
	void SetBase(size_t);
	bool isAlive() const;

private:
	size_t base_;
};