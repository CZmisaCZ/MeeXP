#include "event.h"
#include "fileIO.h"

std::vector<unsigned long long > toUpdate;
std::vector<UserXP*> UserXPs;

auto needXPforLvl(auto arrayPos)
{
	return ((5 * pow(UserXPs.at(arrayPos)->lvl, 2) + 50 * UserXPs.at(arrayPos)->lvl + 100));
}

// add user to list that is waiting for adding XP
bool addXP(dpp::user user)
{
	for (auto i = 0; i < toUpdate.size(); i++)
		if (toUpdate.at(i) == user.id)return 1;

	toUpdate.push_back(user.id);
	return 0;
}

void checkLvlUp(int at)
{
	if (UserXPs.at(at)->xp >= needXPforLvl(at))UserXPs.at(at)->lvl++;
}

// adds XP to user unsing his positin in <vector>UserXPs, mee6 bot like XP calculation
void addRandomXP(int at)
{
	UserXPs.at(at)->xp += 15 + rand() % 10;
	checkLvlUp(at);
}

// add user to database
void addUser(unsigned long long a)
{
	UserXP* newUserXP = new UserXP;
	newUserXP->userID = a;

	UserXPs.push_back(newUserXP);
}

// every minute add xp to users if they messaged within past minute, to avoid getting XP for spamming messages
void applyXP()
{
	//for (auto ii = 0; ii < toUpdate.size(); ii++)
	//	for (auto i = 0; i < UserXPs.size(); i++)
	//		if (UserXPs.at(i)->user == toUpdate.at(ii))
	//			addRandomXP(i);

	std::vector<bool> add;
	for (auto ii = 0; ii < toUpdate.size(); ii++)
	{
		add.push_back(0);

		for (auto i = 0; i < UserXPs.size(); i++)
			if (UserXPs.at(i)->userID == toUpdate.at(ii))
			{
				addRandomXP(i);
				add.at(ii) = 1;
			}			
	}

	// add users that are not in database
	for (auto i = 0; i < add.size(); i++)
		if (add.at(i) == 0)
			addUser(toUpdate.at(i));

	add.clear();

	toUpdate.clear();
}

unsigned long long getXP(dpp::user user)
{
	for (auto i = 0; i < UserXPs.size(); i++)
		if (UserXPs.at(i)->userID == user.id)return UserXPs.at(i)->xp;

	return 0;
}

std::vector<UserXP*> getDatabase()
{
	return UserXPs;
}

void setDatabase(std::vector<UserXP*> data)
{
	UserXPs = data;
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

//returns users position in leaderboard
RankData getRank(unsigned long long ID)
{
	bool loop = true;
	auto num = 0;
	for (auto i = 0; i < UserXPs.size() && loop == true; i++)
		if (UserXPs.at(i)->userID == ID) { num = i; loop = false; }

	auto rank = 1;
	for (auto i = 0; i < UserXPs.size(); i++)
	{
		if (UserXPs.at(i)->xp < UserXPs.at(num)->xp)rank++;
	}

	return RankData(ID, UserXPs.at(num)->xp, needXPforLvl(num), rank, UserXPs.size(), UserXPs.at(num)->lvl);
}