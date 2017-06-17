#pragma once
#include <vector>
#include "Contants.h"
#include "IBattleshipGameAlgo.h"

using namespace std;

class Board3D
{
public:
	vector<vector<vector<char>>> m_board;
	int m_Rows;
	int m_Cols;
	int m_Depth;
	Board3D(int rows, int cols, int depth, int initialChar = BLANK);
	Board3D();
	void PrintBoard(ostream& stream) const;
	char ChatAt(Coordinate& cor) const;
private:
	void AllocateVectorSizes();
	void SetInitialChar(char initialChar = BLANK);
};
