#include "GameController.h"
#include "IFileDirectoryUtils.h"
#include "Contants.h"
#include "3DBoardLoader.h"
#include <thread>
#include <sstream>
#include "GameBoardUtils.h"

GameContoller::GameContoller(Configuration& config): active_threads(0), config(config), IsReportResolts(false), IsGameFinished(false)
{
}

void GameContoller::RunSingleThread(int id)
{
	int tempAT = ++active_threads;
	stringstream at;
	at << "Number of Active Thread : " << tempAT << endl;
	MainLogger.SyncPrint(at);

	thread::id currentThreadId = this_thread::get_id();
	stringstream ss;
	GameTask task;
	bool result;
	do {
		ss.str(string());
		ss << currentThreadId << ": Gettign element from queue" << endl;
		MainLogger.SyncPrint(ss);

		result = GetTaskElement(task);

		if (!result)
		{
			ss.str(string());
			ss << currentThreadId << ": Element is null" << endl;
			MainLogger.SyncPrint(ss);
		}
		else
		{
			ss.str(string());
			ss << currentThreadId << ": Element isn't null. TaskID: " << task.task_id << endl;
			MainLogger.SyncPrint(ss);
			RunSingleGame(task);
		}
	} while (result);

	ss.str(string());
	tempAT = --active_threads;
	
	ss << currentThreadId << ": Exit. Number  remain active threads " << tempAT << endl;
	MainLogger.SyncPrint(ss);

	if (tempAT == 0)
		IsGameFinished = true;	
}


void GameContoller::RunSingleGame(GameTask& gameTask) 
{
	bool isExistEmptyQueue = false;
	GameResultInfo result = gameTask.RunTask();

	PlayerResultElement playerAScore(result.Winner == gameTask.index1, result.Winner == gameTask.index2, result.Points_PlayerA, result.Points_PlayerB, gameTask.index1, gameTask.task_id);
	PlayerResultElement playerBScore(result.Winner == gameTask.index2, result.Winner == gameTask.index1, result.Points_PlayerB, result.Points_PlayerA, gameTask.index2, gameTask.task_id);
	{
		lock_guard<mutex>  lg(result_mutex_); 
		stringstream ss;
		ss << "Inserting to queue: PlayerId-" << playerAScore.PlayerId << "Is win:" << playerAScore.IsWon << " Is Loss" << playerAScore.IsLoss << endl;
		ss << "Inserting to queue: PlayerId-" << playerBScore.PlayerId << "Is win:" << playerBScore.IsWon << " Is Loss" << playerBScore.IsLoss << endl;
		MainLogger.SyncPrint(ss);


		results[gameTask.index1].push(playerAScore);
		results[gameTask.index2].push(playerBScore);

		for each(const queue<PlayerResultElement>& q in results)
		{
			if (q.empty())
			{
				isExistEmptyQueue = true;
				break;
			}
		}
		IsReportResolts = !isExistEmptyQueue;
	}
	if(!isExistEmptyQueue)
	{
		cv.notify_one();
	}
}

bool GameContoller::GetTaskElement(GameTask& task)
{
	std::lock_guard<std::mutex> guard(task_mutex);
	//cout << m_taskList.size() << endl;
	if (!m_taskList.empty())
	{
		task = (move(m_taskList.front()));
		m_taskList.pop();
		return true;
	}
	return false;
}

void GameContoller::ReportManager()
{
	Logger ReporterLogger;
	GameBoardUtils::InitLogger(ReporterLogger, "C:\\temp\\Foo1\\ReporterLoger.log");
	bool stopThreadExecution = false;

	while (!stopThreadExecution)
	{
		vector<PlayerResultElement> elementsToHandle;
		{
			std::unique_lock<std::mutex> lk(result_mutex_);
			cv.wait(lk, [this] {return IsReportResolts || IsGameFinished; });

			IsReportResolts = false;
			if (IsGameFinished)
			{
				ReporterLogger << "Starting reporting - Game finished" << endl;

				for (int i = 0; i < results.size(); ++i)
				{
					FillAllElementsFromQueue(elementsToHandle, i);
				}

				// Stop reporting thread execution in case of end game
				stopThreadExecution = true;
			}
			else
			{
				ReporterLogger << "Starting reporting - Game is not finished" << endl;
				int minElem = MAXINT;

				for each (const queue<PlayerResultElement>& q in results)
				{
					minElem = static_cast<int>(min(minElem, q.size()));
				}
				ReporterLogger << "Min element: " << minElem << endl;

				for (int i = 0; i < results.size(); ++i)
				{
					ReporterLogger << "Queue size before extracting: " << results[i].size() << endl;
					FillElementFromQueue(elementsToHandle, i, minElem);
					ReporterLogger << "Queue size after extracting: " << results[i].size() << endl;

				}
			}
			// Handle updating and reporting
			ReporterLogger << "Release lock and handling " << elementsToHandle.size() << " elements" << endl;
			PlayerScoreUtils::UpdatePlayerScores(PlayerScoreInfos, elementsToHandle, ReporterLogger.logFile);

			lk.unlock();
		}
		
		
	}
}

void GameContoller::FillElementFromQueue(vector<PlayerResultElement>& collectionToFill, int playerId, int elementsToExtract)
{
	queue<PlayerResultElement>& playerResultQueue = results[playerId];

	for (int i = 0; i < elementsToExtract; ++i)
	{
		if (playerResultQueue.empty())
			break;

		PlayerResultElement temp = playerResultQueue.front();
		collectionToFill.push_back(move(temp));
		playerResultQueue.pop();
	}
}

void GameContoller::FillAllElementsFromQueue(vector<PlayerResultElement>& collectionToFill, int playerId)
{
	queue<PlayerResultElement>& playerResultQueue = results[playerId];

	while (!playerResultQueue.empty())
	{
		PlayerResultElement temp = playerResultQueue.front();
		collectionToFill.push_back(move(temp));
		playerResultQueue.pop();
	}
}


void GameContoller::RunApplication()
{
	int result = InitGameController();
	if(result != 0)
	{
		MainLogger.logFile << "Failed to initialize game contorller end of Application" << endl;
		return;
	}

	PrintGameControllerInfo();

	MainLogger.logFile << "Starting filling task queue" << endl;
	result = GenerateGameQueue(); 
	MainLogger.logFile << "Successfully GenerateGameQueue" << endl;

	int id = 0;
	vector<thread> threads(config.thread_num);
	for (auto & t : threads)
	{
		t = thread(&GameContoller::RunSingleThread, this,id);
		++id;
	}

	thread report_thread(&GameContoller::ReportManager, this);
	for (thread & t : threads) { 
		t.join();
	}
	report_thread.join();
}

int GameContoller::InitGameController()
{
	bool result = ConfigurePath();
	if (!result)
	{
		return ErrorExitCode;
	}

	MainLogger.logFile << "Starting board configuration" << endl;
	result = ConfigureBoards();
	if(!result)
	{
		MainLogger.logFile << "Failed to ConfigureBoards" << endl;
		return ErrorExitCode;
	}	

	MainLogger.logFile << "Starting DLL configuration" << endl;
	result = ConfigureDll();

	if (!result)
	{
		MainLogger.logFile << "Failed to ConfigureDll" << endl;
		return ErrorExitCode;
	}

	MainLogger.logFile << "Successfully Initlizlized Game Controller" << endl;
	return 0;
}

bool GameContoller::ConfigureDll()
{
	vector<string> dll_names;

	int cnt = IFileDirectoryUtils::GetAllFiles(config.path, "*.dll", dll_paths, &dll_names);
	MainLogger.logFile << "Found " << cnt << " Dll's" << endl;
	
	if (cnt < 2)
	{
		MainLogger.logFile << "Missing *.dll file. Found " << cnt << endl;
		cout << "Missing an algorithm (dll) file looking in path: " << config.path << endl;
		return false;
	}
	

	for (int i=0; i<dll_paths.size();++i)
	{
		DllAlgo tempAlgo;
		bool result = tempAlgo.LoadDll(dll_paths[i], dll_names[i]);
		if(result)
		{
			algos_factory.push_back(move(tempAlgo));
			results.push_back(queue<PlayerResultElement>());

			// Init player score infos
			PlayerScoreInfos.push_back(PlayerScoreInfo(i, dll_names[i]));
		}
	}

	int index = 0;
	MainLogger << "===== Printing Factories=======" << endl;
	for each (const DllAlgo& algo in algos_factory)
	{
		MainLogger << index << ") Path: " << algo.path << " Name: " << algo.DllName << endl;

		++index;
	}
	MainLogger << "========= End of factory content printing ==============" << endl;

	if (algos_factory.size() < 2)
	{
		return false;
	}

	return true;
}

// Return true is find a least one valid board
bool GameContoller::ConfigureBoards()
{
	BoardLoader board_loader(board3_ds, config.path);
	board_loader.LoadAllBoards();
	MainLogger.logFile << "List Game board size is " << board3_ds.size() << endl;
	return board3_ds.size() != 0;
}

bool GameContoller::GenerateGameQueue()
{
	MainLogger << "============== GenerateGameQueue ==============" << endl;
	int numOfAlgos = static_cast<int>(algos_factory.size());
	int task_id = 0;
	for each (const Board3D& gameBoard in board3_ds)
	{
		for (int i = 1; i < numOfAlgos; i++)
		{
			for (int j = 0; j < numOfAlgos; j++)
			{
				int a = j; int b = (j + i) % numOfAlgos;
				MainLogger << "Inserting to queue player1-" << a << " player2-" << b << " game_id " << task_id << endl;
				m_taskList.emplace(a, b, gameBoard, algos_factory, task_id);
				++task_id;
			}
		}
	}

	MainLogger.logFile << "Task list contains : " << m_taskList.size() << " tasks." << endl;
	return true;
}

void GameContoller::PrintGameControllerInfo() const 
{
	cout << "Number of legal boards : <" << board3_ds.size() << ">" << endl;
	cout << "Number of legal players : <" << algos_factory.size() << ">" << endl;
}

bool GameContoller::ConfigurePath() const
{
	if (!config.path.empty())
	{
		MainLogger.logFile << "Path provided in the argument is " << config.path << endl;
		config.path = IFileDirectoryUtils::GetFullPath(config.path);
		if (IFileDirectoryUtils::DirExists(config.path))
		{
			MainLogger.logFile << "Full path directory exist: " << config.path << endl;
			return true;
		}
		MainLogger.logFile << "Path is not exist " << config.path << endl;
		cout << "Wrong Path: " << config.path << endl;
		return false;
	}
	MainLogger.logFile << "Path is not specified in the argument, getting current working directory" << endl;
	config.path = IFileDirectoryUtils::GetCurrentWorkingDirectory();

	if (IFileDirectoryUtils::DirExists(config.path))
	{
		MainLogger.logFile << "Current working directory is " << config.path << endl;
		return true;
	}
	cout << "Wrong Path: " << config.path << endl;
	return false;
}