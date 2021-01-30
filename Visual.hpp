#pragma once
#include "Memory.hpp"
#include "Offsets.hpp"
#include "Local.hpp"
struct GlowStruct
{
	float R;
	float G;
	float B;
	float A;
	byte padding[16];
	bool rwo;
	bool rwuo;
};
class visual
{

public:
	
	void GlowEsp();
	void update(LocalPlayer* pl);


private:
	LocalPlayer* lp_;
	void makeGlow(GlowStruct glowstruct, DWORD glowObj, DWORD PlayerGlow, float r, float g, float b, float a);
	void updateGlowInfo(LocalPlayer* Entity);
}; extern visual g_Visual;