#pragma once
#include "IArgumentParser.h"
#include <vector>
#include "Board3D.h"
#include "DllAlgo.h"
#include "GameTask.h"
#include <queue>

class GameContoller
{
public:
	GameContoller(Configuration& config);
	void RunSingleThread(int id);
	void PrintGameQueue();
	void RunApplication();

private:
	Configuration& config;
	vector<Board3D> board3_ds;
	vector<string> dll_paths;
	vector<DllAlgo> algos_factory;
	queue<GameTask> m_taskList;
	mutex task_mutex;

	int InitGameController();
	bool ConfigurePath() const;
	bool ConfigureDll();
	bool ConfigureBoards();

	bool GenerateGameQueue();

	void PrintGameControllerInfo() const;

	void RunSingleGame(GameTask& gameTask);
	bool GetTaskElement(GameTask& task);
};
