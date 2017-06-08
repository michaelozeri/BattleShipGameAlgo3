#include "GameBoard.h"

GameBoard::~GameBoard()
{
	for (int i = 0; i < GameBoard::rows(); i++)
	{
		for (int j = 0; j < GameBoard::cols(); j++)
		{
			delete m_gameboard[i][j];
		}
	}
}

void GameBoard::dispose()
{
	for (size_t i = 0; i < rows(); i++)
	{
		for (size_t j = 0; j < cols(); j++)
		{
			delete m_gameboard[i][j];
		}
	}
	delete m_gameboard;
}

char GameBoard::charAt(Coordinate c) const {
	char charAtPosition = m_gameboard[c.row][c.col][c.depth];
	bool isMyChar = GameBoardUtils::IsPlayerIdChar(_playerNum, charAtPosition);
	if (isMyChar) {
		return charAtPosition;
	}
	return ' ';
}

GameBoard::GameBoard(char*** board, int rows, int cols, int depth): m_gameboard(board){
	_rows = rows;
	_cols = cols;
	_depth = depth;
}

