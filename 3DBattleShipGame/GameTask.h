#pragma once
#include "Board3D.h"
#include "IBattleshipGameAlgo.h"
#include <memory>
#include "DllAlgo.h"

class GameTask {
public:
	unique_ptr<IBattleshipGameAlgo> algo1;
	unique_ptr<IBattleshipGameAlgo> algo2;

	Board3D m_gameBoard;

	GameTask(int index1, int index2, Board3D gameBoard, vector<DllAlgo>& factory);
	~GameTask();
};
