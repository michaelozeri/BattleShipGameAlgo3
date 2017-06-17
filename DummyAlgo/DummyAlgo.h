#pragma once
#include "IBattleshipGameAlgo.h"
#include "IOLib.h"

class DummyAlgo : public IBattleshipGameAlgo
{
public:
	Logger AlgoLogger;

	int row;
	int col;
	int depth;
	int _PlayerId;

	const BoardData* _board;
	DummyAlgo();
	static int GetRandNum();
	void setBoard(const BoardData& board) override; // called once to notify player on his board
	void PrintBoard(const BoardData& board);
	void setPlayer(int player) override;
	Coordinate attack() override; // ask player for his move
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override; // notify on last move result
	~DummyAlgo();
};
