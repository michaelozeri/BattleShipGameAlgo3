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


IBattleshipGameAlgo* GetAlgorithm()
{
	return new DummyAlgo();
}

DummyAlgo::DummyAlgo(): row(0), col(0), depth(0), _board(nullptr)
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
	row = 0;
	col = 0;
	depth = 0;
	_board = &board;
	AlgoLogger << "Set Board: (" << board.rows() << ", " << board.cols() << ", " << board.depth() << ")" << endl;
	PrintBoard(board);
}

void DummyAlgo::PrintBoard(const BoardData& board)
{
	for (int k = 0; k < board.depth(); ++k)
	{
		for (int i = 0; i < board.rows(); i++)
		{
			for (int j = 0; j < board.cols(); j++)
			{
				AlgoLogger << board.charAt(Coordinate(i, j, k));
			}
			AlgoLogger.logFile << endl;
		}

		AlgoLogger.logFile << endl;
	}
}

void DummyAlgo::setPlayer(int player)
{
	AlgoLogger << "Set Player " << player << endl;
	_PlayerId = player;
}

Coordinate DummyAlgo::attack()
{
	Coordinate c(-1, -1, -1);
	bool isCountinue = true;

	int k = depth;
	int i = row;
	int j = col;

	for(; k<_board->depth(); ++k)
	{
		for (; i < _board->rows(); ++i)
		{
			for (; j < _board->cols(); ++j)
			{
				if(!isCountinue)
				{
					row = i; col = j; depth = k;
					AlgoLogger << "Attacking on " << c.row << " " << c.col << " " << c.depth << endl;
					return c;
				}
				c = Coordinate(i, j, k);
				if (!GameBoardUtils::IsPlayerIdChar(_PlayerId, _board->charAt(c)))
				{
					isCountinue = false;
					c = Coordinate(i + 1, j + 1, k + 1);
				}
			}
			j = 0;
		}
		i = 0;
	}
	row = _board->rows(); col = _board->cols(); depth = _board->depth();
	AlgoLogger << "Attacking on " << c.row << " " << c.col << " " << c.depth << endl;
	return c;
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

