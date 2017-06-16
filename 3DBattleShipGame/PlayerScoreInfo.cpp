#include "PlayerScoreInfo.h"

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
}


