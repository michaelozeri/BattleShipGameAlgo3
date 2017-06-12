#include "GameController.h"
#include "IFileDirectoryUtils.h"
#include "Contants.h"
#include "3DBoardLoader.h"

GameContoller::GameContoller(Configuration& config): config(config)
{

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

	MainLogger.logFile << "Starting filling task queue" << endl;
	result = GenerateGameQueue();

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

	for each (Board3D gameBoard in board3_ds)
	{
		for (int i = 1; i < numOfAlgos; i++)
		{
			for (int j = 0; j < numOfAlgos; j++)
			{
				m_taskList.emplace(j, (j + i) % numOfAlgos, gameBoard, algos_factory);
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
