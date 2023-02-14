#include "fileIO.h"
#include "settings.h"
#include <iostream>
#include <climits>

bool is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

// find which file is leatest saved in database, bool true for newest, bool false oldest
int whichone(bool leatest)
{
	//whichone(bool leatest)
	int tmp = 0;
	unsigned long long savedwhen=0;
	int filenum=0;
	if (leatest == false)savedwhen = INT_MAX;

	std::ifstream myfile;

	for (auto i = 0; i < sett::backupsnumber; i++)
	{
		myfile.open("./data/"+std::to_string(i)+".cfg", std::ifstream::in);

		std::string line;
		if (myfile)
		{
			if (is_empty(myfile) == false)
			{
				getline(myfile, line);
				if (leatest==true)
				{
					if (stoull(line) > savedwhen) { savedwhen = stoull(line); filenum = i; }
				}
				else
				{
					if(stoull(line) < savedwhen) { savedwhen = stoull(line); filenum = i; }
				}
			}
			else
			{
				if(sett::print)printf(("corrupted file: ./data/" + std::to_string(i) + ".cfg\nreplacing with empty one\n").c_str());
				std::ofstream myfile2;
				myfile2.open("./data/" + std::to_string(i) + ".cfg", std::ofstream::out);
				myfile2 << std::to_string(tmp)+"\n&end\n";
				tmp++;
				myfile2.close();
			}
		}
		else
		{
			if(sett::print)printf(("file missing: ./data/" + std::to_string(i) + ".cfg\nreplacing with empty one\n").c_str());
			std::ofstream myfile2;
			myfile2.open("./data/" + std::to_string(i) + ".cfg", std::ofstream::out);
			myfile2 << std::to_string(tmp) + "\n&end\n";
			tmp++;
			myfile2.close();
		}


		myfile.close();
	}
	return filenum;
}

// loads leatest database from file and retruns it as std::vector<UserXP*>
std::vector<UserXP*> loadData()
{
	std::vector<UserXP*> load;
	std::ifstream myfile;
	myfile.open("./data/"+std::to_string(whichone(true))+".cfg", std::ifstream::in);
	std::string line;
	if (myfile)
	{
		getline(myfile, line);
		getline(myfile, line);
		while (line != "&end")
		{
			UserXP* newUserXP = new UserXP;
			newUserXP->userID = stoull(line);
			getline(myfile, line);
			newUserXP->xp = stoull(line);
			getline(myfile, line);
			newUserXP->messages = stoull(line);
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
		if(sett::print)printf("Could not load database\n");
	}
	return load;
}

//file time is used for number that tracks which file was saved leatest, -1 = default
void saveData(std::vector<UserXP*> UserXPs, unsigned long long time)
{
	unsigned long long filenum = whichone(false);

	std::ifstream myfile2;
	myfile2.open("./data/" + std::to_string(filenum) + ".cfg", std::ifstream::in);
	std::string line;
	getline(myfile2, line);
	if (time == -1) time = stoull(line);
	myfile2.close();

	time +=sett::backupsnumber;
	std::ofstream myfile;
	myfile.open("./data/"+std::to_string(filenum)+".cfg", std::ofstream::out);
	if (myfile)
	{
		myfile << std::to_string(time)+"\n";

		for (auto i = 0; i < UserXPs.size(); i++)
			myfile << UserXPs.at(i)->userID << "\n" << UserXPs.at(i)->xp<<"\n"<< UserXPs.at(i)->messages << "\n" << UserXPs.at(i)->lvl << "\n" << UserXPs.at(i)->sielent << "\n";
	
		myfile << "&end\n";
		myfile.close();

		//if(sett::print)printf("saved\n");
	}
	else
	{
		if(sett::print)printf("Could not save database\n");
	}

	if (time >= 18000000000000000000 - sett::backupsnumber)ressetFileDates();
}

// reset file dates funciton, to avoid file date owerflow
void ressetFileDates()
{
	for (auto i = 0; i < sett::backupsnumber; i++)
	{
		saveData(loadData(),i);
	}
}
