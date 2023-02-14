#include "event.h"
#include "fileIO.h"
#include <iostream>
#include "settings.h"

std::vector<UserXP*> UserXPs;

// check how many xp in total is needed for next lvl (including lvls before)
unsigned long long getXPforLvl(short lvl)
{
	unsigned long long xp = 0;

	for (auto i = 0; i < lvl; i++)
	{
		xp += ((5 * pow(i, 2) + 50 * i + 100));
	}

	return xp;
}

// checks if user has enough xp for lvl up, if yes then increases level and returns 1 else retrun 0
bool checkLvlUp(unsigned long long at)
{
	if (UserXPs.at(at)->xp >= getXPforLvl(UserXPs.at(at)->lvl+1))
	{
		UserXPs.at(at)->lvl++;

		return 1;
	}
	else
		return 0;
}

// adds XP to user unsing his positin in <vector>UserXPs, mee6 bot like XP calculation
void addRandomXP(unsigned long long at)
{
	if (UserXPs.at(at)->xp < 18446744073709551615-(25*sett::globalxpmultiplayer))
		UserXPs.at(at)->xp += (15 + rand() % 10) * sett::globalxpmultiplayer;
	else
		if(sett::print)printf(("user: " + std::to_string(UserXPs.at(at)->userID) + " has reached xp limit, what a no lifer.\n").c_str());
}

// add user to database
void addUser(unsigned long long ID)
{
	if (UserXPs.size()==18446744073709551615)
	{
		UserXP* newUserXP = new UserXP;
		newUserXP->userID = ID;

		UserXPs.push_back(newUserXP);
	}
	else
	{
		if (sett::print)printf("user database is full, limit18446744073709551615\n");
	}
}

//get user pos in database using ID
unsigned long long finduserAT(dpp::user user)
{
	//get user pos in database using ID
	bool loop = true;
	auto num = 0;
	for (auto i = 0; i < UserXPs.size() && loop == true; i++)
		if (UserXPs.at(i)->userID == user.id) { num = i; loop = false; }

	//if user not found then add new
	if (loop == true)
	{
		if (user.is_bot() == false)
		{
			num = UserXPs.size();
			addUser(user.id);
		}
	}

	return num;
}

// add user to list that is waiting for adding XP
bool addXP(dpp::user user)
{
	// apply xp to user
	for (auto i = 0; i < UserXPs.size(); i++)
		if (UserXPs.at(i)->userID == user.id)
		{
			if (UserXPs.at(i)->updated == false)
			{
				UserXPs.at(i)->updated = true;
				addRandomXP(i);
				if (UserXPs.at(i)->messages< 18446744073709551615)UserXPs.at(i)->messages++;else printf(("user " + std::to_string(UserXPs.at(i)->userID)+ " has reached meessages limit: 18446744073709551615\n").c_str());
				return checkLvlUp(i);
			}
		}

	// find it user exists in a database
	for (auto i = 0; i < UserXPs.size(); i++)
		if (UserXPs.at(i)->userID==user.id)return 0;

	// if not add him and apply xp
	if (user.is_bot() == false)
	{
		addUser(user.id);
		addRandomXP(UserXPs.size()-1);
		return checkLvlUp(UserXPs.size()-1);
	}
	return 0;
}


// every minute, reset timers to avoid getting XP for spamming messages
void applyXP()
{
	for (auto i = 0; i < UserXPs.size(); i++)
		UserXPs.at(i)->updated = false;
}

unsigned long long getXP(dpp::user user)
{
	for (auto i = 0; i < UserXPs.size(); i++)
		if (UserXPs.at(i)->userID == user.id)return UserXPs.at(i)->xp;

	return 0;
}

// return users with highest ranks, short for how many
std::vector<UserXP*> getTopDatabase(short howmany)
{
	std::vector<UserXP*> top;
	unsigned long long current=0;
	for (auto i = 0; i < howmany && i < UserXPs.size(); i++)
	{
		UserXP* newUser = new UserXP;
		top.push_back(newUser);

		for (auto ii = 0; ii < UserXPs.size(); ii++)
		{
			if (UserXPs.at(ii)->xp >= current)
			{
				bool thereisone = false;
				for (auto iii = 0; iii < top.size(); iii++)
				{
					if (UserXPs.at(ii)->userID == top.at(iii)->userID)thereisone = true;
				}
				if (thereisone == false)
				{
					current = UserXPs.at(ii)->xp;
					top.at(i) = UserXPs.at(ii);
				}
			}
		}
		current = 0;
	}

	return top;
}

//use user ID to find him in database and funcion returns "RankData" that are all nescessary data for building embeds or messages
RankData getRank(dpp::user user)
{
	//get user pos in database using ID
	bool loop = true;
	auto num = 0;
	for (auto i = 0; i < UserXPs.size() && loop == true; i++)
		if (UserXPs.at(i)->userID == user.id) { num = i; loop = false; }

	//if user not found then add new
	if (loop == true)
	{
		if (user.is_bot() == false)
		{
			num = UserXPs.size();
			addUser(user.id);
		}
		else
		{
			// if bot retrun "empty"
			return RankData(user.id, 0, 0, 0, 0, UserXPs.size(), 0);
		}
	}

	//calc user rank
	auto rank = 1;
	for (auto i = 0; i < UserXPs.size(); i++)
	{
		if (UserXPs.at(i)->xp < UserXPs.at(num)->xp)rank++;
	}

	unsigned long long nextLVL = getXPforLvl(UserXPs.at(num)->lvl + 1);
	unsigned long long currLVL = getXPforLvl(UserXPs.at(num)->lvl);

	return RankData(user.id, UserXPs.at(num)->xp, currLVL, nextLVL, rank, UserXPs.size(), UserXPs.at(num)->lvl);
}

std::vector<UserXP*> getDatabase()
{
	return UserXPs;
}

void setDatabase(std::vector<UserXP*> data)
{
	UserXPs = data;
}

void setXP(dpp::user user, double XP)
{
	//to prevent giving negative xp
	if (XP < 0)XP = 0;

	//get user pos in database using ID
	auto num = finduserAT(user);

	if (user.is_bot() == false)
	{
		//sets desired xp and resets lvl
		UserXPs.at(num)->xp = XP;
		UserXPs.at(num)->lvl = 0;

		//gives lvls back, based on given xp
		while (UserXPs.at(num)->xp >= getXPforLvl(UserXPs.at(num)->lvl + 1))
			UserXPs.at(num)->lvl++;
	}
}

void giveXP(dpp::user user, double XP)
{
	//to prevent giving negative xp
	if (XP < 0)XP = 0;

	//get user pos in database using ID
	auto num = finduserAT(user);

	if (XP + UserXPs.at(num)->xp < 0)
	if (user.is_bot() == false)
	{
		//sets desired xp and resets lvl
		UserXPs.at(num)->xp = XP + UserXPs.at(num)->xp;
		UserXPs.at(num)->lvl = 0;

		//gives lvls back, based on given xp
		while (UserXPs.at(num)->xp >= getXPforLvl(UserXPs.at(num)->lvl + 1))
			UserXPs.at(num)->lvl++;
	}
}

void sielence(dpp::user user)
{
	unsigned long long at = finduserAT(user);

	if (UserXPs.at(at)->sielent == true)UserXPs.at(at)->sielent = false; else UserXPs.at(at)->sielent = true;
}

bool ismoderator(dpp::user user)
{
	for (auto i = 0; i < sett::admins.size(); i++)
		if (user.id == sett::admins.at(i))
			return true;

	return false;
}