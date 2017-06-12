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
	void RunApplication();

private:
	Configuration& config;
	vector<Board3D> board3_ds;
	vector<string> dll_paths;
	vector<DllAlgo> algos_factory;
	queue<GameTask> m_taskList;

	int InitGameController();
	bool ConfigurePath() const;
	bool ConfigureDll();
	bool ConfigureBoards();

	bool GenerateGameQueue();

	void PrintGameControllerInfo() const;

};
