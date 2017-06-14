#pragma once
#include "Board3D.h"

class BoardValidator
{
public:
	static BoardFileErrorCode ValidateBoard(Board3D  boardToValidate);
};
