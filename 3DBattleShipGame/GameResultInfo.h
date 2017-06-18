#pragma once

class GameResultInfo
{
public:
	int Winner;
	int Points_PlayerA;
	int Points_PlayerB;

	GameResultInfo(int winner, int points_playerA, int points_playerB): 
			Winner(winner), Points_PlayerA(points_playerA), Points_PlayerB(points_playerB)
	{
	}
};

class PlayerResultElement
{
public:
	bool IsWon;
	bool IsLoss;
	int PtsFor;
	int PtsAgainst;
	int PlayerId;
	int game_id;

	PlayerResultElement(bool isWon, bool isLoss, int ptsFor, int ptsAgainst, int player_id, int gameID) :
		IsWon(isWon), IsLoss(isLoss), PtsFor(ptsFor), PtsAgainst(ptsAgainst), PlayerId(player_id), game_id(gameID)
	{
	}
};