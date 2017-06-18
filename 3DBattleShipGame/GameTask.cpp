#include "GameTask.h"
#include "GameManager.h"

GameTask::GameTask(): index1(0), index2(0), task_id(0), m_gameBoard()
{
}

GameTask::GameTask(int index1, int index2, Board3D gameBoard, vector<DllAlgo>& factory, int id)
	: index1(index1), index2(index2), algo1(factory[index1].GetAlgoInstance()), algo2(factory[index2].GetAlgoInstance()),
	task_id(id), m_gameBoard(gameBoard)
{
}

GameResultInfo GameTask::RunTask()
{
	GameManager game_manager(move(algo1), move(algo2), m_gameBoard, task_id);
	GameResultInfo res =  game_manager.RunGame();
	if(res.Winner!=NoWinner)
	{
		res.Winner = res.Winner == PlayerAWinner ? index1 : index2;
	}
	MainLogger << "TaskId: " << task_id << " player_id_Winner: " << res.Winner << endl;
	return res;
}