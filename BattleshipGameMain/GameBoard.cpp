#include "GameBoard.h"

GameBoard::GameBoard(char*** board): m_gameboard(board)
{

}

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

char GameBoard::charAt(Coordinate c) const {
	char charAtPosition = m_gameboard[c.row][c.col][c.depth];
	bool isMyChar = GameBoardUtils::IsPlayerIdChar(_playerNum, charAtPosition);
	if (isMyChar) {
		return charAtPosition;
	}
	return ' ';
}