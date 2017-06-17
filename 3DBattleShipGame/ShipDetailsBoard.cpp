#include "ShipDetailsBoard.h"

ShipDetailsBoard::ShipDetailsBoard(Board3D& board3_d, int playerID) : playerID(playerID), mainboard(board3_d), RubberBoatCells(0), RocketShipCells(0), SubmarineCells(0), DestroyeCells(0), negativeScore(0)
{
	for (size_t k = 0; k < mainboard.m_Depth; k++)
	{
		for (size_t i = 0; i < mainboard.m_Rows; i++)
		{
			for (size_t j = 0; j < mainboard.m_Cols; j++)
			{
				char cell = board3_d.m_board[i][j][k];
				if (_utils.IsPlayerIdChar(playerID, cell))
				{
					switch (tolower(cell))
					{
					case RubberBoatB:
						RubberBoatCells++;
						break;
					case RocketShipB:
						RocketShipCells++;
						break;
					case SubmarineB:
						SubmarineCells++;
						break;
					case DestroyerB:
						DestroyeCells++;
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

/**
* \brief Attack on the current player
* \param attack
* \return AtackResult enum instance and update the board
*/
AttackResult ShipDetailsBoard::GetAttackResult(Coordinate attack)
{
	AttackResult result = AttackResult::Miss;
	char cell = mainboard.m_board[attack.row][attack.col][attack.depth];
	if (_utils.IsPlayerIdChar(playerID, cell))
	{
		mainboard.m_board[attack.row][attack.col][attack.depth] = HIT_CHAR;
		result = IsSinkRecursiveChecker(attack.row, attack.col, attack.depth) ? AttackResult::Sink : AttackResult::Hit;
		switch (tolower(cell))
		{
		case 'b':
			RubberBoatCells--;
			negativeScore += result == AttackResult::Sink ? RubberBoatPoints : 0;
			break;
		case 'p':
			RocketShipCells--;
			negativeScore += result == AttackResult::Sink ? RocketShipPoints : 0;
			break;
		case 'm':
			SubmarineCells--;
			negativeScore += result == AttackResult::Sink ? SubmarinePoints : 0;
			break;
		case 'd':
			DestroyeCells--;
			negativeScore += result == AttackResult::Sink ? DestroyerPoints : 0;
			break;
		default:
			// Restore the previous value - should not get here
			mainboard.m_board[attack.row][attack.col][attack.depth] = cell;
			break;
		}
	}
	return result;
}

bool ShipDetailsBoard::IsLoose() const
{
	int sum = RubberBoatCells + RocketShipCells + SubmarineCells + DestroyeCells;
	return (sum == 0);
}

bool ShipDetailsBoard::IsSinkRecursiveChecker(int row, int col, int depth, Direction dir) const
{
	if (row < 0 || row >= mainboard.m_Rows || col < 0 || col >= mainboard.m_Cols || depth < 0 || depth >= mainboard.m_Depth)
	{
		return  true;
	}
	char cell = mainboard.m_board[row][col][depth];
	if (cell == BLANK)
	{
		return true;
	}

	if (cell != SINK_CHAR && cell != HIT_CHAR)
	{
		return false;
	}

	switch (dir)
	{
	case Direction::Right:
		return IsSinkRecursiveChecker(row, col + 1, depth, Direction::Right);
		break;
	case Direction::Left:
		return IsSinkRecursiveChecker(row, col - 1, depth, Direction::Left);
		break;
	case Direction::Up:
		return IsSinkRecursiveChecker(row - 1, col, depth, Direction::Up);
		break;
	case Direction::Down:
		return IsSinkRecursiveChecker(row + 1, col, depth, Direction::Down);
		break;
	case Direction::Outside:
		return IsSinkRecursiveChecker(row, col, depth - 1, Direction::Outside);
		break;
	case Direction::Inside:
		return  IsSinkRecursiveChecker(row, col, depth + 1, Direction::Inside);
		break;
	case Direction::All:
		return IsSinkRecursiveChecker(row, col + 1, depth, Direction::Right) && IsSinkRecursiveChecker(row, col - 1, depth, Direction::Left) &&
			IsSinkRecursiveChecker(row - 1, col, depth, Direction::Up) && IsSinkRecursiveChecker(row + 1, col, depth, Direction::Down) &&
			IsSinkRecursiveChecker(row,col, depth -1, Direction::Outside) && IsSinkRecursiveChecker(row, col, depth + 1, Direction::Inside);
		break;
	}
	return  true;
}