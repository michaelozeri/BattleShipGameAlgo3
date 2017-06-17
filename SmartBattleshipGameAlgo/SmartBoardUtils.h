#pragma once

#include "Board3D.h"

class SmartBoardUtils
{
public:
	static void MarkCannotAttack(int playernum, Board3D& mainBoard, int rows, int cols, int depth);
};