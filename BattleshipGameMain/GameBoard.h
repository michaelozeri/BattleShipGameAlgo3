#pragma once

#include "IBattleshipGameAlgo.h"
#include "../Common/GameBoardUtils.h"

class GameBoard : public BoardData {
public:
	GameBoard(char*** board,int rows,int cols,int depth);
	~GameBoard() ;
	void dispose();
	char charAt(Coordinate c) const ; //returns only selected players' chars
	char*** m_gameboard;
	int _playerNum;
};