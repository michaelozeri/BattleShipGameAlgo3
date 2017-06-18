#include "GameBoardUtils.h"
#include <vector>
#include "IBattleshipGameAlgo.h"
#include "IFileDirectoryUtils.h"
#include "IArgumentParser.h"
#include "3DBoardLoader.h"
#include "GameController.h"
#include <ctime>
#include <iterator>
#include <random>

Logger MainLogger;

using namespace std;

void RunTest();

int main(int argc, char* argv[]) 
{
	//RunTest();
	//return 0;
	

	Configuration config;
	bool result =  IArgumentParser::ParseArguments(config, argc, argv);
	if (!result)
		return 0;
	GameBoardUtils::InitLogger(MainLogger, config.path + "\\GameManager.log");

	GameContoller gameController(config);

	gameController.RunApplication();

	MainLogger.LoggerDispose();
	return 0;
}

#pragma region Test

void FileDirectoryUtils_GetCurrentWorkingDirectory_PositiveScenario()
{
	GameBoardUtils::InitLogger(MainLogger, "FileDirectoryTest.log");

	cout << "Current working directory is " << IFileDirectoryUtils::GetCurrentWorkingDirectory() << endl;

	MainLogger.LoggerDispose();
}

void FileDirectoryUtils_GetFullPath_PositiveScenario()
{
	GameBoardUtils::InitLogger(MainLogger, "GetFullPathTest.log");

	cout << "Full Path is " << IFileDirectoryUtils::GetFullPath("C:\\Users\\mzaitler\\Downloads\\DynamicLoading") << endl;

	MainLogger.LoggerDispose();
}

void FileDirectoryUtils_DirExists_PositiveScenario()
{
	GameBoardUtils::InitLogger(MainLogger, "GetFullPathTest.log");

	cout << "DirExists " << IFileDirectoryUtils::DirExists("C:\\Users\\mzaitler\\Downloads\\DynamicLoading") << endl;

	MainLogger.LoggerDispose();
}

void FileDirectoryUtils_GetAllFiles_PositiveScenario()
{
	GameBoardUtils::InitLogger(MainLogger, "GetAllFiles.log");
	vector<string> matchFile;
	cout << "Num of match file found " << IFileDirectoryUtils::GetAllFiles("C:\\temp\\New folder", "*.dll", matchFile) << endl;

	for (auto itr = matchFile.begin(); itr != matchFile.end(); ++itr)
		cout << *itr << endl;

	MainLogger.LoggerDispose();
}
#pragma endregion 

#pragma region Test_BoardLoader

void BoardLoader_LoadAllBoard()
{	
	GameBoardUtils::InitLogger(MainLogger, "BoardLoader_LoadAllBoard.log");

	vector<Board3D> board3_ds;
	string path = "C:\\Temp\\Foo1";
	BoardLoader* board_loader = new BoardLoader(board3_ds, path);
	board_loader->LoadAllBoards();
	delete board_loader;

	for each (const Board3D& b in board3_ds)
	{
		b.PrintBoard(MainLogger.logFile);
	}

	MainLogger.LoggerDispose();

}

#pragma endregion 

void RunTest()
{
	BoardLoader_LoadAllBoard();
	
}