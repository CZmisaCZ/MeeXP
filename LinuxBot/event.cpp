#include "event.h"
#include "fileIO.h"

std::vector<dpp::user> toUpdate;
std::vector<UserXP*> UserXPs;

// add user to list that is waiting for adding XP
bool addXP(dpp::user user)
{
	for (auto i = 0; i < toUpdate.size(); i++)
		if (toUpdate.at(i).id == user.id)return 1;

	toUpdate.push_back(user);
	return 0;
}

// adds XP to user unsing his positin in <vector>UserXPs, mee6 bot like XP calculation
void addRandomXP(int at)
{
	UserXPs.at(at)->xp += 15 + rand() % 10;
}

// every minute add xp to users if they messaged within past minute, to avoid getting XP for spamming messages
void applyXP()
{
	for (auto ii = 0; ii < toUpdate.size(); ii++)
		for (auto i = 0; i < UserXPs.size(); i++)
			if (UserXPs.at(i)->user == toUpdate.at(ii))
				addRandomXP(i);

	toUpdate.clear();
}

void addUser(dpp::user user)
{
	UserXP* newUserXP = new UserXP;
	newUserXP->user = user;

	UserXPs.push_back(newUserXP);
}

unsigned long long getXP(dpp::user user)
{
	for (auto i = 0; i < UserXPs.size(); i++)
		if (UserXPs.at(i)->user.id == user.id)return UserXPs.at(i)->xp;

	return 0;
}

std::vector<UserXP*> getDatabase()
{
	return UserXPs;
}