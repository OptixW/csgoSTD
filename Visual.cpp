#include "Visual.hpp"
#include <iostream>
#include <thread>
#include <functional>

void visual::updateGlowInfo(std::shared_ptr<LocalPlayer>& Entity)
{
	auto GlowObj = mem.RPM<int>(init::client_dll + signatures::dwGlowObjectManager);

	auto my_team = lp_->getTeam();
	auto ent_team = Entity->getTeam();
	auto ent_glow_index = Entity->getMyGlowIndex();
	if (my_team != ent_team && (my_team == 3 || my_team == 2) && !Entity->isDormant())
	{
		GlowStruct myglow = mem.RPM<GlowStruct>(GlowObj + (ent_glow_index * 0x38) + 0x4);
		makeGlow(myglow, GlowObj, ent_glow_index, 0.1, 0.2, 0.5, 1);
	}
}
void visual::makeGlow(GlowStruct glowstruct, DWORD glowObj, DWORD PlayerGlow, float r, float g, float b, float a)
{
	glowstruct.r = r;
	glowstruct.g = g;
	glowstruct.b = b;
	glowstruct.a = a;
	glowstruct.m_bRenderWhenOccluded = true;
	glowstruct.m_bRenderWhenUnoccluded = false;
	mem.WPM<GlowStruct>((glowObj + (PlayerGlow * 0x38) + 0x4), glowstruct);
	Sleep(1);
}

void visual::GlowEsp()
{
	std::shared_ptr<LocalPlayer> lp(new LocalPlayer());
	for (auto i = 0; i < 16; i++)
	{
		lp->SetBase(mem.RPM<size_t>(init::client_dll + signatures::dwEntityList + i * 0x10));
		updateGlowInfo(lp);
	}
}

void visual::update(std::shared_ptr<LocalPlayer>& pl)
{
	lp_ = pl;
}