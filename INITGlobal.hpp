#pragma once
#define IN_GAME 6
#include <iostream>
#include <memory>
#include "Offsets.hpp"
#include "Aimbot.hpp"
LocalPlayer* getEntityByCrosshairID(int crosshairID, int client);

extern int client_dll;
extern int engine_dll;
extern void initialization();




