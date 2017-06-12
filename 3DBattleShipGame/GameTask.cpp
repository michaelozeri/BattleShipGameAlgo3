#include "GameTask.h"


GameTask::GameTask(int index1, int index2, Board3D gameBoard, vector<DllAlgo>& factory) : algo1(factory[index1].GetAlgoInstance()), algo2(factory[index2].GetAlgoInstance()), m_gameBoard(gameBoard)
{
	
}

GameTask::~GameTask()
{
}
