#include "GameController.h"
#include "IFileDirectoryUtils.h"
#include "Contants.h"
#include "3DBoardLoader.h"
#include <thread>
#include <sstream>

GameContoller::GameContoller(Configuration& config): config(config)
{

}

void GameContoller::RunSingleThread(int id)
{
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
	ss << currentThreadId << ": Exit" << endl;
	MainLogger.SyncPrint(ss);
}

void GameContoller::PrintGameQueue()
{
	// Cannot be implemented
}

void GameContoller::RunSingleGame(GameTask& gameTask) 
{
	gameTask.RunTask();
}


bool GameContoller::GetTaskElement(GameTask& task)
{
	std::lock_guard<std::mutex> guard(task_mutex);
	cout << m_taskList.size() << endl;
	if (!m_taskList.empty())
	{
		task = (move(m_taskList.front()));
		m_taskList.pop();
		return true;
	}
	return false;
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

	PrintGameQueue();

	int id = 0;
	vector<thread> threads(config.thread_num);
	for (auto & t : threads)
	{
		t = thread(&GameContoller::RunSingleThread, this,id);
		++id;
	}

	for (auto & t : threads) {
		t.join();
	}
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
	int cnt = IFileDirectoryUtils::GetAllFiles(config.path, "*.dll", dll_paths);
	MainLogger.logFile << "Found " << cnt << " Dll's" << endl;

	if (cnt < 2)
	{
		MainLogger.logFile << "Missing *.dll file. Found " << cnt << endl;
		cout << "Missing an algorithm (dll) file looking in path: " << config.path << endl;
		return false;
	}
	
	for each (const string& dll in dll_paths)
	{
		DllAlgo tempAlgo;
		bool result = tempAlgo.LoadDll(dll);
		if(result)
		{
			algos_factory.push_back(move(tempAlgo));
		}
	}

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
	int numOfAlgos = static_cast<int>(algos_factory.size());
	int task_id = 0;
	for each (const Board3D& gameBoard in board3_ds)
	{
		for (int i = 1; i < numOfAlgos; i++)
		{
			for (int j = 0; j < numOfAlgos; j++)
			{
				m_taskList.emplace(j, (j + i) % numOfAlgos, gameBoard, algos_factory, task_id);
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
