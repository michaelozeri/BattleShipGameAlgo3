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