#include "fileIO.h"
#include "event.h"
#include <iostream>

void merge(std::vector<UserXP*> a, std::vector<UserXP*> b)
{

};

std::vector<UserXP*> loadData(std::vector<UserXP*> UserXPs)
{
	UserXPs.clear();

	std::ifstream myfile;
	std::string tmp = "./XPdata.cfg";
	myfile.open(tmp.c_str(), std::ifstream::in);
	std::string line;
	if (myfile)
	{
		getline(myfile, line);
		if (line != "\n")
		{
			UserXP* newUserXP = new UserXP;
			newUserXP->user.id = stoi(line);
			getline(myfile, line);
			newUserXP->xp = stoi(line);

			UserXPs.push_back(newUserXP);
		}
	}
	else
	{
		printf("Could not load database");
	}

	return UserXPs;
}

void saveData(std::vector<UserXP*> UserXPs)
{
	std::ofstream myfile;
	std::string tmp = "./XPdata.cfg";
	myfile.open(tmp.c_str(), std::ofstream::out);
	std::string line;
	if (myfile)
	{
		for (auto i = 0; i < UserXPs.size(); i++)
			myfile << UserXPs.at(i)->user.id << "\n" << UserXPs.at(i)->xp;
	
		myfile.close();
	}
	else
	{
		printf("Could not save database");
	}
}