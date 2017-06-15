#include "DummyAlgo.h"
#include "GameBoardUtils.h"

/**
* \brief Return instance of NaiveBattleshipGameAlgo
* \return NaiveBattleshipGameAlgo initalized object - used for loading DLL
*/

IBattleshipGameAlgo* GetAlgorithm()
{
	return new DummyAlgo();
}

DummyAlgo::DummyAlgo()
{
	stringstream fileName;
	fileName << "C:\\Temp\\Foo1\\" << this_thread::get_id() << "_" << "DummyAlgo" << ".log";
	GameBoardUtils::InitLogger(AlgoLogger, fileName.str());
}

void DummyAlgo::setBoard(const BoardData& board)
{
	AlgoLogger << "Set Board: (" << board.rows() << ", " << board.cols() << ", " << board.depth() << ")" << endl;
}

void DummyAlgo::setPlayer(int player)
{
	AlgoLogger << "Set Player " << player << endl;
}

Coordinate DummyAlgo::attack()
{
	return Coordinate(1, 1, 1);
}

std::ostream& operator<< (std::ostream & out, AttackResult const& data)
{
	string s;
	switch (data)
	{
	case AttackResult::Miss:
		s = "Miss";
		break;
	case AttackResult::Hit:
		s = "Hit";
		break;
	case AttackResult::Sink:
		s = "Sink";
		break;
	}
	out << s;
	return out;
}

void DummyAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{
	AlgoLogger << "notifyOnAttackResult: player: " << player << "Coordinate: (" << move.row << ", " << move.col << ", " << move.depth << ")" << " Result: " << result << endl;
}

DummyAlgo::~DummyAlgo()
{
	AlgoLogger << "In Dtor" << endl;
}

