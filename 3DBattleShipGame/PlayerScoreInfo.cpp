#include "PlayerScoreInfo.h"

PlayerScoreInfo::PlayerScoreInfo(int playerId, std::string name): PlayerId(playerId), GamePlayed(0), Wins(0), Losses(0), PtsFor(0), PtsAgainst(0)
{
	// Init player name
	Name = move(name);
}
