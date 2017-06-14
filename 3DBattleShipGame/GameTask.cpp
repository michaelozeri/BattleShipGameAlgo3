#include "GameTask.h"


GameTask::GameTask(int index1, int index2, Board3D gameBoard, vector<DllAlgo>& factory, int id) : algo1(factory[index1].GetAlgoInstance()), algo2(factory[index2].GetAlgoInstance()), m_gameBoard(gameBoard), task_id(id)
{
	
}

GameTask::~GameTask()
{
}
