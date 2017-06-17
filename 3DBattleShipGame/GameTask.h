#pragma once
#include "Board3D.h"
#include "IBattleshipGameAlgo.h"
#include <memory>
#include "DllAlgo.h"
#include "GameResultInfo.h"

class GameTask {
public:
	int index1;
	int index2;
	unique_ptr<IBattleshipGameAlgo> algo1;
	unique_ptr<IBattleshipGameAlgo> algo2;
	int task_id;
	Board3D m_gameBoard;
	GameTask();
	GameTask(int index1, int index2, Board3D gameBoard, vector<DllAlgo>& factory, int id);

	GameResultInfo RunTask();
};