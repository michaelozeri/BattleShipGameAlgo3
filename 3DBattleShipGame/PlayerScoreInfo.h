#pragma once
#include <string>

using namespace std;
class PlayerScoreInfo
{
public:
	int PlayerId;
	std::string Name;
	
	int GamePlayed;
	int Wins;
	int Losses;
	int PtsFor;
	int PtsAgainst;

	PlayerScoreInfo(int playerId, std::string playerName);
};
