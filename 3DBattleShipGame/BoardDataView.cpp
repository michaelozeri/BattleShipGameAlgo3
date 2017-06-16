#include "BoardDataView.h"
#include "GameBoardUtils.h"

BoardDataView::BoardDataView(Board3D board, int playerID) : board3_d_(move(board)), playerId(playerID)
{
	// Set protected vars in base class
	_cols = board.m_Cols;
	_rows = board.m_Rows;
	_depth = board.m_Depth;
}

char BoardDataView::charAt(Coordinate c) const
{
	char cell = board3_d_.ChatAt(c);
	bool res = GameBoardUtils::IsPlayerIdChar(playerId, cell);
	if (res)
		return cell;
	return BLANK;
}
