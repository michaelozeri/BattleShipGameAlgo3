#include "GameTask.h"
#include "GameManager.h"


GameTask::GameTask(int index1, int index2, Board3D gameBoard, vector<DllAlgo>& factory, int id) 
: index1(index1), index2(index2), algo1(factory[index1].GetAlgoInstance()), algo2(factory[index2].GetAlgoInstance()),
                      task_id(id), m_gameBoard(gameBoard)
{
	
}

GameTask::~GameTask()
{
}

void GameTask::RunTask()
{
	GameManager game_manager(move(algo1), move(algo2), m_gameBoard, task_id);
	game_manager.RunGame();
}