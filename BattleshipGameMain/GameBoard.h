#pragma once

#include "IBattleshipGameAlgo.h"
#include "../Common/GameBoardUtils.h"

class GameBoard : BoardData {
public:
	GameBoard(char*** board);
	~GameBoard() ;
	char charAt(Coordinate c) const ; //returns only selected players' chars

	int rows()  const { return _rows; }
	int cols()  const { return _cols; }
	int depth() const { return _depth; }
protected:
	int _playerNum;
	int _rows; 
	int _cols ; 
	int _depth ; 
	char*** m_gameboard;
};