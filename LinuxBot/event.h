#pragma once

#include <vector>
#include <string>
#include <dpp/dpp.h>
#include "settings.h"

struct UserXP
{
	unsigned long long userID;
	unsigned long long xp = 0;
	short lvl = 0;
	bool sielent = sett::sielent;
	bool updated = 0;
};

struct RankData
{
	RankData(unsigned long long userID, unsigned long long xp, unsigned long long xptothislvl, unsigned long long xptonextlvl, unsigned long long rank, unsigned long long maxrank, short lvl)
	{
		this->userID = userID;
		this->xp = xp;
		this->xptothislvl = xptothislvl;
		this->xptonextlvl = xptonextlvl;
		this->rank = rank;
		this->maxrank = maxrank;
		this->lvl = lvl;
	}

	unsigned long long userID;
	unsigned long long xp;
	unsigned long long xptothislvl;
	unsigned long long xptonextlvl;
	unsigned long long rank;
	unsigned long long maxrank;
	short lvl;
};

std::vector<UserXP*> getDatabase();
bool addXP(dpp::user user);
unsigned long long getXP(dpp::user user);
void applyXP();
void setDatabase(std::vector<UserXP*> data);
std::vector<UserXP*> getTopDatabase(short howmany);
RankData getRank(dpp::user user);
void giveXP(dpp::user user, double XP);
void setXP(dpp::user user, double XP);
void sielence(dpp::user user);
bool ismoderator(dpp::user user);