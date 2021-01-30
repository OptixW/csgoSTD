#pragma once
#include "Memory.hpp"
#include "Offsets.hpp"
#include "Local.hpp"
struct GlowStruct
{
	float r = 1.f;
	float g = 1.f;
	float b = 1.f;
	float a = 1.f;
	char pad[16];
	bool m_bRenderWhenOccluded = true; //0x24
	bool m_bRenderWhenUnoccluded = false; //0x25
	bool m_bFullBloomRender = false; //0x26
};
class visual {

public:
	
	void GlowEsp();
	void update(LocalPlayer* pl);


private:
	LocalPlayer* lp_;
	void makeGlow(GlowStruct glowstruct, DWORD glowObj, DWORD PlayerGlow, float r, float g, float b, float a);
	void updateGlowInfo(LocalPlayer* Entity);
}; extern visual g_Visual;