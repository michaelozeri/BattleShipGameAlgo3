#pragma once

#include "../BattleshipGameMain/DllAlgo.h"
#include "../BattleshipGameMain/GameBoard.h"

class GameTask {
public:
	DllAlgo m_playerA, m_playerB;
	GameBoard m_gameBoard;
	GameTask(DllAlgo& playerA,DllAlgo& playerB,GameBoard& gameBoard);
	~GameTask();
	void playGame();
};