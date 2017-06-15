#pragma once
#include  "IBattleshipGameAlgo.h"
#include "Board3D.h"

class BoardDataView : public BoardData
{
private:
	Board3D board3_d_;
	int playerId;
public:
	BoardDataView(Board3D board, int playerID);
	char charAt(Coordinate c) const override; //returns only selected players' chars
};
