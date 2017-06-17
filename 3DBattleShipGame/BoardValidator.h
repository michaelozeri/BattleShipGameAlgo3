#pragma once
#include "Board3D.h"
#include "IOLib.h"
#include "GameBoardUtils.h"

using namespace std;

class BoardValidator
{
public:
	static bool ValidateBoard(Board3D  boardToValidate,Logger& mainLogger);
};
