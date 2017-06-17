#include "Board3D.h"
#include <ostream>

using namespace std;

Board3D::Board3D(int rows, int cols, int depth, int initialChar): m_Rows(rows), m_Cols(cols), m_Depth(depth)
{
	AllocateVectorSizes();
	SetInitialChar(initialChar);
}

void Board3D::AllocateVectorSizes()
{
	m_board.resize(m_Rows);
	for (int i = 0; i < m_Rows; ++i) 
	{
		m_board[i].resize(m_Cols);
		for (int j = 0; j < m_Cols; ++j)
		{
			m_board[i][j].resize(m_Depth);
		}
	}
}

void Board3D::SetInitialChar(char initialChar)
{
	for (int i=0; i<m_Rows; ++i)
	{
		for (int j=0; j<m_Cols; ++j)
		{
			for(int k=0; k<m_Depth; ++k)
			{
				m_board[i][j][k] = initialChar;
			}
		}
	}
}

Board3D::Board3D(): m_Rows(0), m_Cols(0), m_Depth(0)
{
}

void Board3D::PrintBoard(ostream& stream) const
{
	for (int k = 0; k < m_Depth; ++k)
	{
		for (int i = 0; i < m_Rows; i++)
		{
			for (int j = 0; j < m_Cols; j++)
			{
				stream << m_board[i][j][k];
			}
			stream << endl;
		}

		stream << endl;
	}
}

char Board3D::ChatAt(Coordinate& cor) const
{
	// Check validity of cor
	if (cor.row < 0 || cor.row >= m_Rows || cor.col < 0 || cor.col >= m_Cols || cor.depth < 0 || cor.depth >= m_Depth)
		return BLANK;

	return m_board[cor.row][cor.col][cor.depth];
}
