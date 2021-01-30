#include "Visual.hpp"
#include <iostream>



void visual::updateGlowInfo(LocalPlayer * Entity)
{
	std::string module_cl = "client.dll";
	DWORD module_ = mem.getModuleBase(module_cl);
	auto GlowObj = mem.RPM<int>(module_ + signatures::dwGlowObjectManager);

	auto my_team = lp_->getTeam();
	auto ent_team = Entity->getTeam();
	auto ent_glow_index = Entity->getMyGlowIndex();
	
	
		GlowStruct myglow = mem.RPM<GlowStruct>(GlowObj + (ent_glow_index * 0x38) + 0x4);
		makeGlow(myglow, GlowObj, ent_glow_index, 0.1, 0, 102, 104);
	
}
void visual::makeGlow(GlowStruct glowstruct,DWORD glowObj, DWORD PlayerGlow, float r, float g, float b, float a)
{
	glowstruct.r = r;
	glowstruct.g = g;
	glowstruct.b = b;
	glowstruct.a = a;
	glowstruct.m_bRenderWhenOccluded = true;
	glowstruct.m_bRenderWhenUnoccluded = false;
	mem.WPM<GlowStruct>((glowObj + (PlayerGlow * 0x38) + 0x4), glowstruct);
	
}

void visual::GlowEsp()
{
	std::string module_cl = "client.dll";
	DWORD module_ = mem.getModuleBase(module_cl);
	LocalPlayer* lp = new LocalPlayer();
	for (auto i = 0; i < 32; i++)
	{
		lp->SetBase(mem.RPM<size_t>(module_ + signatures::dwEntityList + i * 0x10));
		updateGlowInfo(lp);
		Sleep(5);
	}
	

}

void visual::update(LocalPlayer* pl)
{
	lp_ = pl;
}
