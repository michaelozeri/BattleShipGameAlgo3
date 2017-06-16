#pragma once

class GameResultInfo
{
public:
	int Winner;
	int Points_PlayerA;
	int Points_PlayerB;

	GameResultInfo(int winner, int points_playerA, int points_playerB): Winner(winner), Points_PlayerA(points_playerA), Points_PlayerB(points_playerB)
	{
	}
};

class PlayerResultElement
{
public:
	bool IsWon;
	int PtsFor;
	int PtsAgainst;
	int PlayerId;

	PlayerResultElement(bool isWon, int ptsFor, int ptsAgainst, int player_id) : IsWon(isWon), PtsFor(ptsFor), PtsAgainst(ptsAgainst), PlayerId(player_id)
	{
	}
};