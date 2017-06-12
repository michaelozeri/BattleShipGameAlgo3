
#include "DummyAlgo.h"



/**
* \brief Return instance of NaiveBattleshipGameAlgo
* \return NaiveBattleshipGameAlgo initalized object - used for loading DLL
*/

IBattleshipGameAlgo* GetAlgorithm()
{
	return new DummyAlgo();
}

void DummyAlgo::setBoard(const BoardData& board)
{
}

void DummyAlgo::setPlayer(int player)
{
}

Coordinate DummyAlgo::attack()
{
	return Coordinate(1, 1, 1);
}

void DummyAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{
}

DummyAlgo::~DummyAlgo()
{
}

