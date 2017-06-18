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
	double Percentage;

	PlayerScoreInfo(int playerId, std::string playerName);
};

class PlayerScoreUtils
{
	//weight definitions
	static const int indexW = 8;
	static const int NameW = 24;
	static const int WinsW = 8;
	static const int LossesW = 8;
	static const int PercentageW = 8;
	static const int PtsForW = 10;
	static const int PtsAgainst = 11;


public:
	static void UpdatePlayerScores(vector<PlayerScoreInfo>& scores, vector<PlayerResultElement>& elementsToUpdate, ostream& out);
	static void PrintScores(ostream& out, vector<PlayerScoreInfo>& scores);
	static void CalaculatePercentage(vector<PlayerScoreInfo>& scores);
	static void PrintSingleScore(ostream& out, const PlayerScoreInfo& score, int list_index);
	static double GeneratePrecentageString(const PlayerScoreInfo& score);
	static void PrintStartLine(ostream& out);
};