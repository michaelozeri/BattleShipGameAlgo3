#pragma once
#include <string>
#include <vector>
#include "GameResultInfo.h"

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

class PlayerScoreUtils
{
public:
	static void UpdatePlayerScores(vector<PlayerScoreInfo>& scores, vector<PlayerResultElement> elementsToUpdate);
};