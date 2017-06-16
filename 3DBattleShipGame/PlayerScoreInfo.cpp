#include "PlayerScoreInfo.h"
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;

PlayerScoreInfo::PlayerScoreInfo(int playerId, std::string name): PlayerId(playerId), GamePlayed(0), Wins(0), Losses(0), PtsFor(0), PtsAgainst(0)
{
	// Init player name
	Name = move(name);
}

void PlayerScoreUtils::UpdatePlayerScores(vector<PlayerScoreInfo>& scores, vector<PlayerResultElement> elementsToUpdate)
{
	for each (const PlayerResultElement& elem in elementsToUpdate)
	{
		PlayerScoreInfo& currentPlayer = scores[elem.PlayerId];
		currentPlayer.GamePlayed++;
		currentPlayer.Wins += elem.IsWon ? 1 : 0;
		currentPlayer.Losses += elem.IsWon ? 0 : 1;
		currentPlayer.PtsFor += elem.PtsFor;
		currentPlayer.PtsAgainst += elem.PtsAgainst;
	}

	PrintScores(cout, scores);
}

void PlayerScoreUtils::PrintScores(ostream& out, const vector<PlayerScoreInfo>& scores)
{
	PrintStartLine(out);
	int list_index = 1;
	for each(const PlayerScoreInfo& score in scores)
	{
		PrintSingleScore(out, score, list_index);
		++list_index;
	}
}

void PlayerScoreUtils::PrintSingleScore(ostream& out, const PlayerScoreInfo& score, int list_index)
{
	stringstream ss;
	ss << list_index << ".";
	out << left << setw(indexW) << ss.str();
	out << left << setw(NameW) << score.Name;
	out << left << setw(WinsW) << score.Wins;
	out << left << setw(LossesW) << score.Losses;
	out << left << setw(PercentageW) << 50;
	out << left << setw(PtsForW) << score.PtsFor;
	out << left << setw(PtsAgainst) << score.PtsAgainst;
	out << endl;
}

void PlayerScoreUtils::PrintStartLine(ostream& out)
{
	out << left << setw(indexW) << "#";
	out << left << setw(NameW) << "Team Name";
	out << left << setw(WinsW) << "Wins";
	out << left << setw(LossesW) << "Losses";
	out << left << setw(PercentageW) << "%";
	out << left << setw(PtsForW) << "Pts For";
	out << left << setw(PtsAgainst) << "Pts Against";
	out << endl << endl;
}