#pragma once

#include "event.h"
#include <fstream>

void saveData(std::vector<UserXP*> UserXPs, unsigned long long time);
std::vector<UserXP*> loadData();
void ressetFileDates();