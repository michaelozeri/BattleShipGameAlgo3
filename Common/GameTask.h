#pragma once

#include "../BattleshipGameMain/DllAlgo.h"
#include "../BattleshipGameMain/GameBoard.h"

class GameTask {
public:
	int m_playerAIndex, m_playerBIndex;
	GameBoard m_gameBoard;
	GameTask(int playerAIndex,int playerBIndex,GameBoard& gameBoard);
	~GameTask();
	void playGame();
};