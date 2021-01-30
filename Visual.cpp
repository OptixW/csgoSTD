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
	
	
		GlowStruct myglow = mem.RPM<GlowStruct>(GlowObj + (ent_glow_index * 0x38) + 4);
		makeGlow(myglow, GlowObj, ent_glow_index, 255, 255, 255, 255);
	
}
void visual::makeGlow(GlowStruct glowstruct,DWORD glowObj, DWORD PlayerGlow, float r, float g, float b, float a)
{
	glowstruct.R = r;
	glowstruct.G = g;
	glowstruct.B = b;
	glowstruct.A = a;
	glowstruct.rwo = true;
	glowstruct.rwuo = false;
	mem.WPM<GlowStruct>((glowObj + (PlayerGlow * 0x38) + 4), glowstruct);
	
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
	}
	

}

void visual::update(LocalPlayer* pl)
{
	lp_ = pl;
}
