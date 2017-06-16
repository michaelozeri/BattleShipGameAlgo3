#pragma once
#include "GameBoardUtils.h"
#include "IBattleshipGameAlgo.h"
#include "Board3D.h"

enum class Direction { Right, Left, Up, Down, Inside, Outside, All };

/*
* a utility class for keeping track of game state
*/
class ShipDetailsBoard
{
	GameBoardUtils _utils;
public:
	int playerID;
	Board3D& mainboard;

	int RubberBoatCells;
	int RocketShipCells;
	int SubmarineCells;
	int DestroyeCells;

	int negativeScore; // Save the score of the other player
	ShipDetailsBoard(Board3D& board, int playerID);
	AttackResult GetAttackResult(Coordinate attack);
	bool IsLoose() const;
	bool IsSinkRecursiveChecker(int row, int col, int depth, Direction dir = Direction::All) const;
 };