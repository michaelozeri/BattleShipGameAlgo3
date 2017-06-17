#pragma once
#include "IArgumentParser.h"
#include <vector>
#include "Board3D.h"
#include "DllAlgo.h"
#include "GameTask.h"
#include <queue>
#include "PlayerScoreInfo.h"
#include "GameResultInfo.h"
#include <atomic>

class GameContoller
{
public:
	GameContoller(Configuration& config);
	void RunApplication();

private:
	volatile atomic<int> active_threads;
	Configuration& config;
	vector<Board3D> board3_ds;
	vector<string> dll_paths;
	vector<DllAlgo> algos_factory;
	queue<GameTask> m_taskList;
	mutex task_mutex;

	vector<queue<PlayerResultElement>> results;
	std::condition_variable cv;
	volatile bool IsReportResolts;
	volatile bool IsGameFinished;
	mutex result_mutex_;
	vector<PlayerScoreInfo> PlayerScoreInfos;

	int InitGameController();
	bool ConfigurePath() const;
	bool ConfigureDll();
	bool ConfigureBoards();

	bool GenerateGameQueue();

	void PrintGameControllerInfo() const;

	void RunSingleGame(GameTask& gameTask);
	bool GetTaskElement(GameTask& task);
	void ReportManager();
	void FillElementFromQueue(vector<PlayerResultElement>& collectionToFill, int playerId, int elementsToExtract) ;
	void FillAllElementsFromQueue(vector<PlayerResultElement>& collectionToFill, int playerId);

	void RunSingleThread(int id);

};
