#pragma once
#include "Board3D.h"
#include <iostream>
#include "../Common/IOLib.h"
#include "../Common/GameBoardUtils.h"

using namespace std;

class BoardValidator
{
public:
	static bool ValidateBoard(Board3D&  boardToValidate,Logger& mainLogger);
};
