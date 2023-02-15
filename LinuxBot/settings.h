#pragma once
#include <vector>
#include <string>

inline namespace sett
{
	inline const std::string BOT_TOKEN = "BOT TOKEN";
	inline int backupsnumber = 3; //number of save backups, 3 is recommended
	inline int saveinterval = 1; //save interval in minutes
	inline std::vector<unsigned long long> admins = { 305444371883491329 };  //<------------------- list of admins discord IDs to use "/givexp"
	inline float globalxpmultiplayer = 1; //xp mutiplyer 2 = 2x amout of xp
	inline bool print = 1; //print data to console, turn off if you are not using this
	inline bool sielent = 1; //1 = every user will have "sielent" mode on default, that means that "level up" embeds will not pritn
}
