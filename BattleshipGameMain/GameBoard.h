#pragma once

#include "IBattleshipGameAlgo.h"
#include "../Common/GameBoardUtils.h"

class GameBoard : BoardData {
public:
	GameBoard(char*** board);
	GameBoard();
	~GameBoard() ;
	char charAt(Coordinate c) const ; //returns only selected players' chars
	char*** m_gameboard;
	int _playerNum;	
};