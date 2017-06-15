#pragma once
#include "IBattleshipGameAlgo.h"
#include "IOLib.h"

class DummyAlgo : public IBattleshipGameAlgo
{
public:
	Logger AlgoLogger;

	DummyAlgo();
	void setBoard(const BoardData& board) override; // called once to notify player on his board
	void setPlayer(int player) override;
	Coordinate attack() override; // ask player for his move
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override; // notify on last move result
	~DummyAlgo();
};
