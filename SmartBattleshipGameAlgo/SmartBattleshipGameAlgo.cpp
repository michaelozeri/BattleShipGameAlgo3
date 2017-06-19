#include "SmartBattleshipGameAlgo.h"
#include "GameBoardUtils.h"
#include <cassert>
#include <vector>     
#include <cstdlib>
#include "SmartBoardUtils.h"

Logger MainLogger;

void SmartBattleshipGameAlgo::setPlayer(int player) {
	m_myPlayerNum = player;
}

SmartBattleshipGameAlgo::SmartBattleshipGameAlgo(): m_numRows(0), m_numCols(0), m_depth(0), m_mode(), m_currDir(), m_myPlayerNum(0)
{
}

Coordinate SmartBattleshipGameAlgo::attack()
{
	// create new coordinate - attackCoor
	// if we are at RandomMode:
	Coordinate attackCoor(AttckDoneIndex, AttckDoneIndex, AttckDoneIndex);
	if (m_mode == AttackMode::RandomMode)
	{
		attackCoor = GetValidRandomAttack();
		if (attackCoor.row == AttckDoneIndex) {
			return attackCoor;
		}
		return AllignCord(attackCoor);
	}

	// if we're at Target Mode
	attackCoor = GetValidOptionalAttack();
	if(attackCoor.row == AttckDoneIndex)
	{
		// Return to random state
		StartRandomAttackMode();
		attackCoor = GetValidRandomAttack();
		if (attackCoor.row == AttckDoneIndex) {
			return attackCoor;
		}
		return AllignCord(attackCoor);
	}
	return AllignCord(attackCoor);
}

Coordinate SmartBattleshipGameAlgo::GetValidRandomAttack ()
{
	while (m_attacksRemain.size() > 0)
	{
		int randomLocation = GetRandom(m_attacksRemain.size());
		Coordinate currAttack = m_attacksRemain[randomLocation];

		if (!IsAttackValid(currAttack.row, currAttack.col, currAttack.depth))
		{
			// erase the element from the vector
			m_attacksRemain.erase(m_attacksRemain.begin() + randomLocation);
			continue;
		}
		else
		{
			return currAttack;
		}
	}
	return Coordinate(AttckDoneIndex, AttckDoneIndex, AttckDoneIndex);
}

Coordinate SmartBattleshipGameAlgo::GetValidOptionalAttack()
{
	// isCheckDir = false in case current direction is unknown
	// but if current direction is not unknown, mean it's rows/cols/depth
	// isCheckDir will be true
	bool isCheckDir = m_currDir != AttackDir::unknown;

	while (m_PotentialAttacks.size() > 0)
	{
		int randomLocation = GetRandom(m_PotentialAttacks.size());
		tuple<int, int, int, AttackDir> currentAttack = m_PotentialAttacks[randomLocation];

		if (!IsAttackValid(get<0>(currentAttack), get<1>(currentAttack), get<2>(currentAttack)))
		{
			m_PotentialAttacks.erase(m_PotentialAttacks.begin() + randomLocation);
		}

		else if (isCheckDir && get<3>(currentAttack) != m_currDir)
		{
			m_PotentialAttacks.erase(m_PotentialAttacks.begin() + randomLocation);
		}

		else
		{
			return Coordinate(get<0>(currentAttack), get<1>(currentAttack), get<2>(currentAttack));
		}
	}
	return Coordinate(AttckDoneIndex, AttckDoneIndex, AttckDoneIndex);
}

void SmartBattleshipGameAlgo::AddSqureCellsToQueue(int row, int col, int depth)
{
	// Add row-1 cell
	if(row > 0)
	{
		AddPotentialAttckIfLegal(row - 1, col, depth, AttackDir::rows_dir);
	}

	// Add row+1 cell
	if(row  < m_numRows -1)
	{
		AddPotentialAttckIfLegal(row + 1, col, depth, AttackDir::rows_dir);
	}

	// Add col-1 cell
	if(col > 0)
	{
		AddPotentialAttckIfLegal(row, col - 1, depth, AttackDir::cols_dir);
	}

	// Add col+1 cell
	if(col < m_numCols -1 )
	{
		AddPotentialAttckIfLegal(row, col + 1, depth, AttackDir::cols_dir);
	}

	// Add depth-1 cell
	if (depth > 0) 
	{
		AddPotentialAttckIfLegal(row, col, depth - 1, AttackDir::depth_dir);
	}

	// Add depth+1 cell
	if (depth < m_depth - 1)
	{
		AddPotentialAttckIfLegal(row, col, depth + 1, AttackDir::depth_dir);
	}
}

void SmartBattleshipGameAlgo::AddPotentialAttckIfLegal(int row, int col, int depth, AttackDir dir)
{
	if(IsAttackValid(row, col, depth))
	{
		m_PotentialAttacks.push_back(make_tuple(row, col, depth, dir));
	}
}

void SmartBattleshipGameAlgo::HandleMyRandomMode(int row, int col, int depth, AttackResult result)
{
	switch (result)
	{
	case AttackResult::Miss:
		// Continue as random mode
		StartRandomAttackMode(); 
		break;
	case AttackResult::Hit:
		// Switch to Target mode
		m_mode = AttackMode::TargetMode;

		// On first hit battleship direcion in unknown
		m_currDir = AttackDir::unknown;

		// Add all around legal cell to queue
		AddSqureCellsToQueue(row, col, depth);
		break;

	case AttackResult::Sink:
		// if we successfully sinked, continue as random mode
		StartRandomAttackMode();
		break;
	}
}

tuple<int, int, int, AttackDir> SmartBattleshipGameAlgo::Dequeue(int row, int col, int depth)
{
	for (vector<tuple<int, int, int, AttackDir>>::iterator itr = m_PotentialAttacks.begin(); itr != m_PotentialAttacks.end(); ++itr)
	{
		int y = get<0>(*itr);
		int x = get<1>(*itr);
		int z = get<2>(*itr);

		if (y == row && x == col && z == depth)
		{
			tuple<int, int, int, AttackDir> retValue = *itr;
			m_PotentialAttacks.erase(itr);
			return retValue;
		}
	}
	return{ 0, 0, 0, AttackDir::unknown };
}

void SmartBattleshipGameAlgo::AddToQueueAfterHit(int row, int col, int depth)
{
	if(m_currDir == AttackDir::rows_dir)
	{
		// Add row-1
		if(row > 0)
		{
			AddPotentialAttckIfLegal(row - 1, col, depth, AttackDir::rows_dir);
		}

		// Add row+1
		if (row < m_numRows - 1)
		{
			AddPotentialAttckIfLegal(row + 1, col, depth, AttackDir::rows_dir);
		}
	}

	else if(m_currDir == AttackDir::cols_dir)
	{
		// Add col-1
		if(col > 0)
		{
			AddPotentialAttckIfLegal(row, col - 1, depth, AttackDir::cols_dir);
		}

		// Add col+1
		if(col < m_numCols -1 )
		{
			AddPotentialAttckIfLegal(row, col + 1, depth, AttackDir::cols_dir);
		}
	}
	else if (m_currDir == AttackDir::depth_dir) {
		// Add depth-1
		if (depth > 0) {
			AddPotentialAttckIfLegal(row, col, depth - 1, AttackDir::depth_dir);
		}

		// Add depth+1
		if (depth < m_depth - 1) {
			AddPotentialAttckIfLegal(row, col, depth + 1, AttackDir::depth_dir);
		}
	}
}

void SmartBattleshipGameAlgo::HandleMyTargetMode(int row, int col, int depth, AttackResult result)
{
	switch (result)
	{
	// in case we miss, we take out the tuple from the queue
	case AttackResult::Miss:
		Dequeue(row, col, depth);
		break;
	case AttackResult::Hit:
	{
		tuple<int, int, int, AttackDir> val = Dequeue(row, col, depth);

		if (m_currDir == AttackDir::unknown)
		{
			m_currDir = get<3>(val);
		}
		AddToQueueAfterHit(row, col, depth);
		break;
	}
	case AttackResult::Sink:
		// Back to random state
		StartRandomAttackMode();
		break;
	}
}

// 
void SmartBattleshipGameAlgo::HandleMyAttackResult(int row, int col, int depth, AttackResult result)
{
	switch (m_mode)
	{
	case AttackMode::RandomMode:
		HandleMyRandomMode(row, col, depth, result);
		break;
	case AttackMode::TargetMode:
		HandleMyTargetMode(row, col, depth, result);
		break;
	}
}

void SmartBattleshipGameAlgo::HandleRivalAttackResult(int row, int col, int depth, AttackResult result)
{
	if (result == AttackResult::Miss)
	{
		char c = m_Board3d.m_board[row][col][depth];
		if (c != CannotAttackShip)
			m_Board3d.m_board[row][col][depth] = CannotAttck;
	}
}

void SmartBattleshipGameAlgo::MarkInvalidCell(int row, int col, int depth, AttackResult result)
{
	// we mark the value of the char at the board
	// in the place that WE(!) attacked
	char markValue = CannotAttck;
	switch (result)
	{
	// if we missed, we mark that place with CannotAttack ('N').
	case AttackResult::Miss:
		markValue = CannotAttck;
		break;
	case AttackResult::Hit:
		// if we hit, markvalue gets 'S'.
		markValue = CannotAttackShip;
		break;
	case AttackResult::Sink:
		// if we Sink, means there's no ship left:
		// first markvalue gets 'S'.
		markValue = CannotAttackShip;
		// then we mark the places around the ship
		// (because if there was a ship there, there can't be another besides it)
		MarkSinkBattleAroundAsInvlid(row, col, depth);
		break;
	}
	m_Board3d.m_board[row][col][depth] = markValue;
}

// this function is operated when we successfully sank the opponent ship
void SmartBattleshipGameAlgo::MarkSinkBattleAroundAsInvlid(int row, int col, int depth)
{
	// if we're out of bound somehow...
	if (row < 0 || row > m_numRows || col < 0 || col > m_numCols || depth < 0 || depth > m_depth) {
		return;
	}
	
	// we take the curCell...
	// then we mark him as CannotAttack
	char curCell = m_Board3d.m_board[row][col][depth];
	m_Board3d.m_board[row][col][depth] = CannotAttck;

	// now we check if curCell is CannotattackShip, if it so, it means there was 
	// an opponent's ship there, so we need to operate recursivly the function on all
	// the points around it...
	if (curCell == CannotAttackShip) {
		MarkSinkBattleAroundAsInvlid(row - 1, col, depth);
		MarkSinkBattleAroundAsInvlid(row + 1, col, depth);
		MarkSinkBattleAroundAsInvlid(row, col - 1, depth);
		MarkSinkBattleAroundAsInvlid(row, col + 1, depth);
		MarkSinkBattleAroundAsInvlid(row, col, depth - 1);
		MarkSinkBattleAroundAsInvlid(row, col, depth + 1);
	}
}

/*
this function is called at startup to update each players board game
*/
void SmartBattleshipGameAlgo::setBoard(const BoardData& board)
{
	// initialize logger:
	MainLogger.InitLogger("SmartAlgo.log");

	// getting fields of board from BoardData.
	m_numRows = board.rows();
	m_numCols = board.cols();
	m_depth = board.depth();
	m_attacksRemain.clear();
	StartRandomAttackMode();

	m_mode = AttackMode::RandomMode; // Starting from random mode

	// creating a Board3D for the smart player, all is BLANK
	m_Board3d = Board3D(m_numRows, m_numCols, m_depth);

	// Copy board
	for (int k = 0; k < m_depth; ++k)
	{
		for (int i = 0; i < m_numRows; ++i)
		{
			for (int j = 0; j < m_numCols; ++j)
			{
				m_Board3d.m_board[i][j][k] = board.charAt(Coordinate(i, j, k));
			}
		}
	}

	// mark on m_Board Y if we can strike and N if we can't
	SmartBoardUtils::MarkCannotAttack(m_myPlayerNum, m_Board3d, m_numRows, m_numCols, m_depth);
	
	// move on the board and inserting the relevant attack 
	// coordinates to the vector m_attacksRemain
	for (int i = 0; i < m_numRows; i++)
	{
		for (int j = 0; j < m_numCols; j++)
		{
			for (int k = 0; k < m_depth; k++) {
	
				if (m_Board3d.m_board[i][j][k] == CanAttck) { // if we can attak the spot.
					// creating new coordinate and
					// insert it to the vector of attacks
					Coordinate AttackCore(i, j, k);
					m_attacksRemain.push_back(AttackCore);
				}
			}
		}
	}
}

// this function "fixes" the coordinates it gets
Coordinate SmartBattleshipGameAlgo::AllignCord(Coordinate currCoor, bool isNegative)
{
	if (currCoor.row != AttckDoneIndex && currCoor.col != AttckDoneIndex && currCoor.depth != AttckDoneIndex)
	{
		// Move to 1-10 cord
		int offset = isNegative ? -1 : 1;
		return{ currCoor.row + offset, currCoor.col + offset, currCoor.depth + offset };
	}
	return currCoor;
}

void SmartBattleshipGameAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{
	// we make a coordinate with relevant offset using AllingCord
	Coordinate currCoor = AllignCord({ move.row, move.col, move.depth }, true);
	// if we're the player that the attack result is relevant to
	if (player == m_myPlayerNum) 
	{
		// marking cells according to the result.
		MarkInvalidCell(currCoor.row, currCoor.col, currCoor.depth, result);

		HandleMyAttackResult(currCoor.row, currCoor.col, currCoor.depth, result);
	}
	else
	{
		HandleRivalAttackResult(currCoor.row, currCoor.col, currCoor.depth, result);
	}
}


// Returns random number between 0 to max number - 1
int SmartBattleshipGameAlgo::GetRandom(size_t maxNumber)
{
	return rand() % maxNumber;
}

// this function start a random attack
void SmartBattleshipGameAlgo::StartRandomAttackMode()
{
	// defines attack mode of smart player to random mode
	m_mode = AttackMode::RandomMode;
	// defines current direction attack to unknown
	m_currDir = AttackDir::unknown;

	// Empty potentail attack collection
	m_PotentialAttacks.clear();
}

// this function checks if we cant attack the coordinate (row, col and depth)
// if the board's value in this coordinate is CannotAttack or CannotAttack ship
// then we return false.
bool SmartBattleshipGameAlgo::IsAttackValid(int row, int col, int depth) const
{
	if (row < 0 || row >= m_numRows || col < 0 || col >= m_numCols || depth < 0 || depth >= m_depth)
		return false;

	char value = m_Board3d.m_board[row][col][depth];
	return value != CannotAttck && value != CannotAttackShip;
}

/**
* \brief Return instance of SmartBattleshipGameAlgo
* \return SmartBattleshipGameAlgo initalized object - used for loading DLL
*/


// and what is the smart algo builder do
IBattleshipGameAlgo* GetAlgorithm()
{
	return (new SmartBattleshipGameAlgo());
}
