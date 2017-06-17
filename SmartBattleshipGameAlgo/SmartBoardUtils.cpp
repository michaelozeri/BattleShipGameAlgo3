#include "Contants.h"
#include "Board3D.h"
#include "SmartBoardUtils.h"
#include "GameBoardUtils.h"

// this function mark all the places on the board 
void SmartBoardUtils::MarkCannotAttack(int playernum, Board3D& mainBoard, int rows, int cols, int depth) {

	char cannotAttckChar = CannotAttck;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < depth; k++) {
				// if the char matches our player, then we mark all the 
				// places around it with CannotAttack char ('N')
				if (GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i][j][k]))
				{
					// mark me
					mainBoard.m_board[i][j][k] = cannotAttckChar;
					//mark col-1
					if ((j > 0) && (!GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i][j - 1][k])))
					{
						mainBoard.m_board[i][j - 1][k] = cannotAttckChar;
					}
					//mark col+1
					if ((j < cols - 1) && (!GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i][j + 1][k])))
					{
						mainBoard.m_board[i][j + 1][k] = cannotAttckChar;
					}
					//mark row-1
					if ((i > 0) && (!GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i - 1][j][k])))
					{
						mainBoard.m_board[i - 1][j][k] = cannotAttckChar;
					}
					//mark row+1
					if ((i < rows - 1) && (!GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i + 1][j][k])))
					{
						mainBoard.m_board[i + 1][j][k] = cannotAttckChar;
					}
					// mark depth-1
					if ((k > 0) && (!GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i][j][k - 1])))
					{
						mainBoard.m_board[i][j][k - 1] = cannotAttckChar;
					}
					//mark depth+1
					if ((k < depth - 1) && (!GameBoardUtils::IsPlayerIdChar(playernum, mainBoard.m_board[i][j][k + 1])))
					{
						mainBoard.m_board[i][j][k + 1] = cannotAttckChar;
					}
				}
			}
		}
	}
	// mark all places at the board where we can hit with 'Y' (CanAttack)
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < depth; k++) {
				if (!(mainBoard.m_board[i][j][k] == cannotAttckChar)) {
					mainBoard.m_board[i][j][k] = CanAttck;
				}
			}
		}
	}
}