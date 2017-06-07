#include "GameManager.h"
#include "../Common/IFileDirectoryUtils.h"
#include "../Common/Contants.h"
#include "../Common/GameBoardUtils.h"
#include "ShipDetailsBoard.h"

GameManager::GameManager(Configuration& config): config(config){}

bool GameManager::ConfigureDlls() 
{
	vector<string> DllPathcollection;
	int cnt = IFileDirectoryUtils::GetAllFiles(config.path, "*.dll", DllPathcollection);
	if (cnt < 2)
	{
		MainLogger.logFile << "Missing *.dll file. Found " << cnt << endl;
		cout << "Missing an algorithm (dll) file looking in path: " << config.path << endl;
		return false;
	}

	for each (string dllPath in DllPathcollection)
	{
		MainLogger.logFile << "DLl path in use is " << dllPath << endl;
		//load dll from file path
		DllAlgo dllAlgo;
		if (!dllAlgo.LoadDll(dllPath)) {
			//TODO: what to do in here in case of err
		}
		
		MainLogger.logFile << "dll was loaded successfuly" << endl;

		//add the gameboard to the vector gameboards
		m_algoArray.push_back(dllAlgo);
	}
		
	MainLogger.logFile << "Loaded: " << cnt << " Dll's from path: " << config.path << endl;
	return true;
}

/*
check if path inserted or not and if ok
 */
bool GameManager::ConfigurePath() const
{
	if(!config.path.empty())
	{
		MainLogger.logFile << "Path provided in the argument is " << config.path << endl;
		config.path = IFileDirectoryUtils::GetFullPath(config.path);
		if(IFileDirectoryUtils::DirExists(config.path))
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
	
	if(IFileDirectoryUtils::DirExists(config.path))
	{
		MainLogger.logFile << "Current working directory is " << config.path << endl;
		return true;
	}
	cout << "Wrong Path: " << config.path << endl;
	return false;
}

bool GameManager::ConfigureBoards()
{
	vector<string> boardPathsCollection;
	int cnt = IFileDirectoryUtils::GetAllFiles(config.path,"*.sboard", boardPathsCollection);
	if(cnt <= 0)
	{
		MainLogger.logFile << "Missing *.sboard file" << endl;
		cout << "Missing board file (*.sboard) looking in path: " << config.path << endl;
		return false;
	}
	for each (string boardPath in boardPathsCollection)
	{
		MainLogger.logFile << "Board path in use is " << boardPath << endl;

		//load main game board from file
		GameBoard gameBoard = GameBoardUtils::CreateGameBoardFromFile(boardPath);

		//validate Game Board
		BoardFileErrorCode errcode = GameBoardUtils::ValidateGameBoard(gameBoard); //TODO: mordi enhence
		if (errcode != BoardFileErrorCode::Success) {
			//TODO: check error code if ok and how to exit
		}

		MainLogger.logFile << "Board was validated successfuly" << endl;
		GameBoardUtils::PrintBoard(MainLogger.logFile, gameBoard);

		//add the gameboard to the vector gameboards
		m_boardArray.push_back(gameBoard);
	}
	return true;
}

bool GameManager::ConfigureFiles()
{
	bool res1 = ConfigureBoards();
	bool res2 = ConfigureDlls();
	return res1 && res2;
}

/*
from previous HW TODO: check if needed
*/
bool GameManager::InitDllAlgo(DllAlgo& algo, const string & path, int playerID) const
{
	bool result  = algo.LoadDll(path);
	if(!result)
	{
		cout << "Cannot load dll: " << path;
		return false;
	}

	// Set Player Board
	char** playerboard = GameBoardUtils::ClonePlayerBoard(const_cast<const char**>(mainGameBoard), playerID,ROWS,COLS);
	algo.algo->setBoard(playerID, const_cast<const char**>(playerboard), ROWS, COLS);
	GameBoardUtils::DeleteBoard(playerboard, ROWS);

	MainLogger.logFile << "Set board for player " << playerID << " finished" << endl;

	// Init
	result = algo.algo->init(config.path);
	if(!result)
	{
		MainLogger.logFile << "Failed to init for player " << playerID << endl;
		cout << "Algorithm initialization failed for dll: " << path << endl;
		return false;
	}
	
	MainLogger.logFile << "Init successfuly for player " << playerID << endl;
	return true;
}

int GameManager::GameInitializer()
{
	bool result = ConfigurePath(); 
	if (!result)
	{
		return ErrorExitCode;
	}

	result = ConfigureFiles();
	if (!result)
	{
		return ErrorExitCode;
	}

	result = InitializeTaskQueue();
	if (!result)
	{
		return ErrorExitCode;
	}
	return 0;
}

Coordinate GameManager::GetNextPlayerAttack(int player_id, IBattleshipGameAlgo*  player_a, IBattleshipGameAlgo* player_b)
{
	if (player_id == PlayerAID)
	{
		return player_a->attack();
	}
	else
	{
		return player_b->attack();
	}
	// Fatal Error
	MainLogger.logFile << "Fatal error occured. Attack move was asked for non exixting player id " << player_id << endl;
	return ;
}


AttackResult GameManager::GetAttackResult(const pair<int, int>& pair, char** board, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB)
{
	return GameBoardUtils::IsPlayerIdChar(PlayerAID, board[pair.first][pair.second]) ? detailsA.GetAttackResult(pair) : detailsB.GetAttackResult(pair);
}

void GameManager::PrintPoints(ShipDetailsBoard& playerA, ShipDetailsBoard& playerB)
{
	cout << "Points:" << endl;
	cout << "Player A: " << playerB.negativeScore << endl;
	cout << "Player B: " << playerA.negativeScore << endl;
}

void GameManager::PrintSinkCharRec(char** maingameboard, Bonus& b, int i, int j, int player)
{
	if (i < 0 || i >= ROWS || j < 0 || j >= COLS) // Stop recursion condition
	{
		return;
	}

	char currentCell = maingameboard[i][j];
	if (currentCell != HIT_CHAR)
		return;

	maingameboard[i][j] = SINK_CHAR;
	b.PrintPlayerChar(maingameboard[i][j], j, i, player);
	PrintSinkCharRec(maingameboard, b, i, j - 1, player);
	PrintSinkCharRec(maingameboard, b, i, j + 1, player);
	PrintSinkCharRec(maingameboard, b, i - 1, j, player);
	PrintSinkCharRec(maingameboard, b, i + 1, j, player);
}

bool GameManager::IsPlayerWon(int currentPlayer, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB)
{
	return currentPlayer == PlayerAID ? detailsB.IsLoose() : detailsA.IsLoose();
}

// Attack pair 1-10
bool GameManager::ValidAttackCor(const pair<int, int>& pair)
{
	return pair.first > 0 && pair.first <= ROWS && pair.second > 0 && pair.second <= COLS;
}


int GameManager::PlayGame() const
{
	ShipDetailsBoard playerAboardDetails(mainGameBoard, PlayerAID);
	ShipDetailsBoard playerBboardDetails(mainGameBoard, PlayerBID);

	int playerIdToPlayNext = PlayerAID;

	// Configure Bonus start point and color
	Bonus bonus(!config.bonusParam.isQuiet, config.bonusParam.delayInMiliseconds);
	bonus.Init(mainGameBoard, ROWS, COLS);
	GameBoardUtils::ChangeFontSize();

	//main game play
	bool AattacksDone = false;
	bool BattacksDone = false;

	// While not both of players ended their attacks
	while (!AattacksDone || !BattacksDone)
	{
		Coordinate tempCoordinate = GetNextPlayerAttack(playerIdToPlayNext, algo1.algo, algo2.algo); 
		if (tempCoordinate.first == AttckDoneIndex && (tempCoordinate.second == AttckDoneIndex))
		{
			switch (playerIdToPlayNext)
			{
			case PlayerAID:
				AattacksDone = true;
				break;
			case PlayerBID:
				BattacksDone = true;
				break;
			default:;
			}

			// Flip players
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
		}
		else if (ValidAttackCor(tempCoordinate))
		{
			//aligned both axis -1 because main board starts from (0,0)
			tempCoordinate = { tempCoordinate.first - 1,tempCoordinate.second - 1 };

			char attckCell = mainGameBoard[tempCoordinate.first][tempCoordinate.second];
			bool isSelfAttack = GameBoardUtils::IsPlayerIdChar(playerIdToPlayNext, attckCell);

			//calculate attack and update mainboard
			AttackResult tempattackresult = GetAttackResult(tempCoordinate, mainGameBoard, playerAboardDetails, playerBboardDetails);

			string resultDesc;
			switch (tempattackresult)
			{
			case AttackResult::Miss: resultDesc = "Miss";  break;
			case AttackResult::Hit: resultDesc = "Hit"; break;
			case AttackResult::Sink:resultDesc = "Sink"; break;
			default: ;
			}

			MainLogger.logFile << "Player " << playerIdToPlayNext << " attack in (" << tempCoordinate.first << "," << tempCoordinate.second << ") result: " << resultDesc << endl;

			//update players - Notify with values 1-10 and not 0-9
			algo1.algo->notifyOnAttackResult(playerIdToPlayNext, tempCoordinate.first + 1, tempCoordinate.second + 1, tempattackresult);
			algo2.algo->notifyOnAttackResult(playerIdToPlayNext, tempCoordinate.first + 1, tempCoordinate.second + 1, tempattackresult);

			if (tempattackresult != AttackResult::Miss)
			{
				int playerTosetColor;
				if (isSelfAttack)
				{
					playerTosetColor = playerIdToPlayNext;
				}
				else
				{
					playerTosetColor = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
				}

				if (tempattackresult == AttackResult::Sink)
				{
					// In case sink update all the cell to SINK_CHAR
					PrintSinkCharRec(mainGameBoard, bonus, tempCoordinate.first, tempCoordinate.second, playerTosetColor);
				}
				else // In case hit update only the target cell
					bonus.PrintPlayerChar(mainGameBoard[tempCoordinate.first][tempCoordinate.second], tempCoordinate.second, tempCoordinate.first, playerTosetColor);
			}

			if (tempattackresult == AttackResult::Miss || isSelfAttack)
			{
				// Flip Players
				playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			}

			if (IsPlayerWon(PlayerAID, playerAboardDetails, playerBboardDetails))
			{
				bonus.Dispose(); // Important: Don't touch and don't change the order of statements [Mordehai]
				cout << "Player A won" << endl;
				PrintPoints(playerAboardDetails, playerBboardDetails);
				return 0;
			}
			if (IsPlayerWon(PlayerBID, playerAboardDetails, playerBboardDetails))
			{
				bonus.Dispose(); // Important: Don't touch and don't change the order of statements [Mordehai]
				cout << "Player B won" << endl;
				PrintPoints(playerAboardDetails, playerBboardDetails);
				return 0;
			}
		}
		else
		{
			MainLogger.logFile << "Invlaid attack <" << tempCoordinate.row << "," << tempCoordinate.col << ","
								<< "> for player " << playerIdToPlayNext << ". Flipping players" << endl;
			// Flip players
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
		}
	}

	bonus.Dispose(); // Important: Don't touch and don't change the order of statements [Mordehai]
	PrintPoints(playerAboardDetails, playerBboardDetails);
	return 0;
}

void GameManager::GameManagerCleanup() const
{
	GameBoardUtils::DeleteBoard(mainGameBoard, ROWS);
	algo1.Dispose();
	algo2.Dispose();

	MainLogger.LoggerDispose();
}

int GameManager::RunGame()
{
	//initialize logger
	GameBoardUtils::InitLogger(MainLogger, "GameManager.log");

	//initialize and validate all boards , dll algo and queue of games
	int code = GameInitializer();
	if(code == ErrorExitCode)
	{
		return code;
	}
	MainLogger.logFile << "===== Game Initilized =======" << endl;

	//This is a must print for the home assignment
	cout << "Number of legal players: " << m_algoArray.size;
	cout << "Number of legal boards: " << m_boardArray.size;
	//start running the game
	code = PlayGame();
	
	MainLogger.logFile << "Game exit code is " << code << endl;

	GameManagerCleanup();
	return code;
}


#pragma region Test

void GameManager::Test_GetAllAttacks() const
{
	pair<int, int> attack = algo1.algo->attack();
	while (attack.first != AttckDoneIndex)
	{
		MainLogger.logFile << attack.first << "," << attack.second << endl;
		attack = algo1.algo->attack();
	}
}

bool GameManager::InitializeTaskQueue() {

	//TODO: finish this

	return true;
}

#pragma endregion 