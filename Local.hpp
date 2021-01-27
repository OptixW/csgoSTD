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
	Vector2 getPunchAngle() const;
	
	void SetBase(size_t);
	bool isAlive();

private:
	size_t base_;
};