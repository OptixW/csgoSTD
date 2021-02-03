#pragma once
#include "Memory.hpp"
#include "Offsets.hpp"
#include "Local.hpp"
#include "IBaseGame.hpp"
struct GlowStruct
{
	float r = 1.f;
	float g = 1.f;
	float b = 1.f;
	float a = 1.f;
	char pad[16];
	bool m_bRenderWhenOccluded = true; 
	bool m_bRenderWhenUnoccluded = false; 
	bool m_bFullBloomRender = false; 
};
class visual {

public:
	
	void GlowEsp();
	void update(LocalPlayer* pl);
	

private:
	void makeGlow(GlowStruct glowstruct, DWORD glowObj, DWORD PlayerGlow, float r, float g, float b, float a);
	void updateGlowInfo(std::shared_ptr<LocalPlayer> Entity);
	 LocalPlayer* lp_;
	 bool active_ = false;
	
}; extern visual g_Visual;