#pragma once

#include <vector>
#include <string>
#include <dpp/dpp.h>

struct UserXP
{
	dpp::user user;
	unsigned long long xp = 0;
};

std::vector<UserXP*> getDatabase();
bool addXP(dpp::user user);
unsigned long long getXP(dpp::user user);
void applyXP();