#pragma once
#include <iostream>
#include <memory>
#include "Memory.hpp"
#include "Offsets.hpp"
#include "Local.hpp"
extern int client_dll;
extern int engine_dll;


[[noreturn]] void initialization();
