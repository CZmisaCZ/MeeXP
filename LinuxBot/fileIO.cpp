#include "fileIO.h"
#include "event.h"
#include <iostream>

std::vector<UserXP*> loadData()
{
	std::vector<UserXP*> load;

	std::ifstream myfile;
	myfile.open("./XPdata.cfg", std::ifstream::in);
	std::string line;
	if (myfile)
	{
		getline(myfile, line);
		while (line != "&end")
		{
			UserXP* newUserXP = new UserXP;
			newUserXP->userID = stoull(line);
			getline(myfile, line);
			newUserXP->xp = stoull(line);
			getline(myfile, line);
			newUserXP->lvl = stoi(line);
			getline(myfile, line);
			newUserXP->sielent = stoi(line);
			getline(myfile, line);

			load.push_back(newUserXP);
		}
	}
	else
	{
		printf("Could not load database\n");
	}
	return load;
}

void saveData(std::vector<UserXP*> UserXPs)
{
	std::ofstream myfile;
	myfile.open("./XPdata.cfg", std::ofstream::out);
	std::string line;
	if (myfile)
	{
		for (auto i = 0; i < UserXPs.size(); i++)
			myfile << UserXPs.at(i)->userID << "\n" << UserXPs.at(i)->xp<< "\n"<<UserXPs.at(i)->lvl<<"\n"<<UserXPs.at(i)->sielent<<"\n";
	
		myfile << "&end\n";
		myfile.close();

		//printf("saved\n");
	}
	else
	{
		printf("Could not save database\n");
	}
}