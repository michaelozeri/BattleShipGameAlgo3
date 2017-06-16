#include "DummyAlgo.h"
#include "GameBoardUtils.h"
#include <atomic>
#include <ctime>
#include <random>
#include <windows.h>

/**
* \brief Return instance of NaiveBattleshipGameAlgo
* \return NaiveBattleshipGameAlgo initalized object - used for loading DLL
*/

atomic<int> instance = -1;

IBattleshipGameAlgo* GetAlgorithm()
{
	++instance;
	return new DummyAlgo(4);
}

DummyAlgo::DummyAlgo(int instancea)
{
	

	stringstream fileName;
	fileName << "C:\\Temp\\Foo1\\" << this_thread::get_id() << "_" << "DummyAlgo" << GetRandNum() << ".log";
	GameBoardUtils::InitLogger(AlgoLogger, fileName.str());
}

int DummyAlgo::GetRandNum () 
{
	std::vector<int> v(100);

	for (int j = 0; j<100; j++)
	{
		v[j] = j;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(v.begin(), v.end(), g);
	return v[0];
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

