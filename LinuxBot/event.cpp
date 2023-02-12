#include "event.h"
#include "fileIO.h"
#include <iostream>

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
bool checkLvlUp(int at)
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
void addRandomXP(int at)
{
	UserXPs.at(at)->xp += 15 + rand() % 10;
}

// add user to database
void addUser(unsigned long long a)
{
	UserXP* newUserXP = new UserXP;
	newUserXP->userID = a;

	UserXPs.push_back(newUserXP);
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
				current = UserXPs.at(ii)->xp;
				top.at(i) = UserXPs.at(ii);
			}
		}
		current = 0;
	}

	return top;
}

//use user ID to find him in database and funcion returns "RankData" that are all nescessary data for building embeds or messages
RankData getRank(unsigned long long ID)
{
	//get user pos in database using ID
	bool loop = true;
	auto num = 0;
	for (auto i = 0; i < UserXPs.size() && loop == true; i++)
		if (UserXPs.at(i)->userID == ID) { num = i; loop = false; }

	//if user not found then add new
	if (loop == true)
	{
		num = UserXPs.size();
		addUser(ID);
	}

	//calc user rank
	auto rank = 1;
	for (auto i = 0; i < UserXPs.size(); i++)
	{
		if (UserXPs.at(i)->xp < UserXPs.at(num)->xp)rank++;
	}

	unsigned long long nextLVL = getXPforLvl(UserXPs.at(num)->lvl + 1);
	unsigned long long currLVL = getXPforLvl(UserXPs.at(num)->lvl);

	float procet = (currLVL - nextLVL) / (UserXPs.at(num)->xp - currLVL);
	

	return RankData(ID, UserXPs.at(num)->xp, nextLVL, rank, UserXPs.size(), UserXPs.at(num)->lvl);
}

std::vector<UserXP*> getDatabase()
{
	return UserXPs;
}

void setDatabase(std::vector<UserXP*> data)
{
	UserXPs = data;
}