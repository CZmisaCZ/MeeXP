#pragma once

#include "event.h"
#include <fstream>

void saveData(std::vector<UserXP*> UserXPs);
std::vector<UserXP*> loadData();