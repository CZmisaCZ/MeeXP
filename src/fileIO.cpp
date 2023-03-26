#include "fileIO.h"
#include "event.h"
#include "settings.h"
#include <iostream>
#include <climits> //for INT_MAX
#include <cstdint> //for unsigned long long
#include <filesystem> //to create folders

void printw(std::string input)
{
	if (sett::print)
		std::cout<<input<<std::endl;

	if (sett::webhook.size()>10)
	{
    	std::string sys ="curl -H \"Content-Type: application/json\" -X POST -d '{\"content\":\"'\""+input + "\"'\"}' " + sett::webhook;
   		std::system(sys.c_str());
	}
}

bool is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

/*#include <sys/stat.h>
inline bool does_file_exist (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}*/

// find which file is leatest saved in database, bool true for newest, bool false oldest
int whichone(bool leatest)
{
	
	//whichone(bool leatest)
	int tmp = 0;
	unsigned long long savedwhen=0;
	int filenum=0;
	if (leatest == false)savedwhen = INT_MAX;
	
	std::ifstream myfile69;
	
	for (int i = 0; i < sett::backupsnumber; i++)
	{
		myfile69.open("./data/"+std::to_string(i)+".cfg", std::ifstream::in);

		std::string line;
		if (myfile69)
		{
			if (is_empty(myfile69) == false)
			{
				getline(myfile69, line);
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
				printw("> :bangbang: corrupted database: ./data/" + std::to_string(i) + ".cfg, replacing with empty one");
				std::ofstream myfile2;
				myfile2.open("./data/" + std::to_string(i) + ".cfg", std::ofstream::out);
				myfile2 << std::to_string(tmp)+"\n&end";
				tmp++;
				myfile2.close();
			}
		}
		else
		{
			printw("> :warning: database missing: ./data/" + std::to_string(i) + ".cfg, replacing with empty one");
			std::ofstream myfile2;
			myfile2.open("./data/" + std::to_string(i) + ".cfg", std::ofstream::out);
			myfile2 << std::to_string(tmp) + "\n&end";
			tmp++;
			myfile2.close();
		}


		myfile69.close();
	}
	return filenum;
}

// loads leatest database from file and retruns it as std::vector<UserXP*>
std::vector<UserXP*> loadData()
{
	std::filesystem::create_directories("./data");

	std::vector<UserXP*> load;
	std::ifstream myfile;
	myfile.open("./data/"+std::to_string(whichone(true))+".cfg", std::ifstream::in);
	std::string line;
	if (myfile)
	{
		getline(myfile, line);
		getline(myfile, line);
        auto placeholder = line != "&end";
        while (placeholder) 
		{
            UserXP* newUserXP = new UserXP;
			newUserXP->userID = (unsigned long long)std::strtoull(line.c_str(),NULL,(int)line.size());
			getline(myfile, line);
			newUserXP->xp = (unsigned long long)std::strtoull(line.c_str(),NULL,(int)line.size());
			getline(myfile, line);
			newUserXP->messages = (unsigned long long)std::strtoull(line.c_str(),NULL,(int)line.size());
			getline(myfile, line);
			newUserXP->lvl = (short)std::strtoull(line.c_str(),NULL,(int)line.size());
			getline(myfile, line);
			newUserXP->sielent = (bool)std::strtoull(line.c_str(),NULL,(int)line.size());
			getline(myfile, line);
			load.push_back(newUserXP);
        }

        printw("> :ballot_box_with_check: Database loaded");
	}
	else
	{
		printw("> :bangbang: Could not load database");
	}
	return load;
}

//file time is used for number that tracks which file was saved leatest, 18446744073709551615 = default
void saveData(std::vector<UserXP*> UserXPs, unsigned long long time)
{
	std::filesystem::create_directories("./data");

	unsigned long long filenum = whichone(false);

	std::ifstream myfile2;
	myfile2.open("./data/" + std::to_string(filenum) + ".cfg", std::ifstream::in);
	std::string line;
	getline(myfile2, line);
	if (time == (unsigned long long)18446744073709551615) time = stoull(line);
	myfile2.close();

	time +=sett::backupsnumber;
	std::ofstream myfile;
	myfile.open("./data/"+std::to_string(filenum)+".cfg", std::ofstream::out);
	if (myfile)
	{
		myfile << std::to_string(time)+"";

		for (auto i = 0; i < UserXPs.size(); i++)
			myfile << UserXPs.at(i)->userID << "" << UserXPs.at(i)->xp<<""<< UserXPs.at(i)->messages << "" << UserXPs.at(i)->lvl << "" << UserXPs.at(i)->sielent << "";
	
		myfile << "&end";
		myfile.close();

		printw("> :ballot_box_with_check: Database saved");
	}
	else
	{
		printw("> :bangbang: Could not save database");
	}

	if (time >= (unsigned long long)18000000000000000000 - sett::backupsnumber)ressetFileDates();
}

// reset file dates funciton, to avoid file date owerflow
void ressetFileDates()
{
	for (unsigned long long i = 0; i < sett::backupsnumber; i++)
	{
		saveData(loadData(),i);
	}
}