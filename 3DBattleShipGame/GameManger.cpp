#include "GameManager.h"
#include "IFileDirectoryUtils.h"
#include "Contants.h"
#include "GameBoardUtils.h"
#include "ShipDetailsBoard.h"
#include "Board3D.h"

GameManager::GameManager(unique_ptr<IBattleshipGameAlgo> algo1, unique_ptr<IBattleshipGameAlgo> algo2, Board3D board, int id) :
	view1(board, PlayerAID), view2(board, PlayerBID), game_id(id),
	algo_ptr1(move(algo1)), algo_ptr2(move(algo2)), mainGameBoard(move(board))
{
}

bool GameManager::InitPlayers()
{
	return InitDllAlgo(PlayerAID) && InitDllAlgo(PlayerBID);
}

bool GameManager::InitDllAlgo( int playerID)
{
	unique_ptr<IBattleshipGameAlgo>::pointer algo_ptr = playerID == PlayerAID ? algo_ptr1.get() : algo_ptr2.get();

	// First Step: Set Player Board - No Return Value
	GameLogger << "Set Player Id with Value: " << playerID << endl;
	algo_ptr->setPlayer(playerID);
	
	// Second Step: SetBoard - No Return Value
	GameLogger.logFile << "Set board for player " << playerID << endl;
	algo_ptr->setBoard(playerID == PlayerAID ? view1 : view2);

	GameLogger.logFile << "Init successfuly for player " << playerID << endl;
	return true;
}

int GameManager::GameInitializer()
{
	bool result = InitPlayers();
	if (!result)
	{
		return ErrorExitCode;
	}

	return 0;
}

pair<int, int> GameManager::GetNextPlayerAttack(int player_id, IBattleshipGameAlgo*  player_a, IBattleshipGameAlgo* player_b)
{
	if (player_id == PlayerAID)
	{
		//return player_a->attack();
	}
	if (player_id == PlayerBID)
	{
		//return player_b->attack();
	}
	// Fatal Error
	GameLogger.logFile << "Fatal error occured. Attack move was asked for non exixting player id " << player_id << endl;
	return{ };
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

/*
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
*/

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
	/*
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
		pair<int, int> tempPair = GetNextPlayerAttack(playerIdToPlayNext, algo1.algo, algo2.algo); 
		if (tempPair.first == AttckDoneIndex && (tempPair.second == AttckDoneIndex))
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
		else if (ValidAttackCor(tempPair))
		{
			//aligned both axis -1 because main board starts from (0,0)
			tempPair = { tempPair.first - 1,tempPair.second - 1 };

			char attckCell = mainGameBoard[tempPair.first][tempPair.second];
			bool isSelfAttack = GameBoardUtils::IsPlayerIdChar(playerIdToPlayNext, attckCell);

			//calculate attack and update mainboard
			AttackResult tempattackresult = GetAttackResult(tempPair, mainGameBoard, playerAboardDetails, playerBboardDetails);

			string resultDesc;
			switch (tempattackresult)
			{
			case AttackResult::Miss: resultDesc = "Miss";  break;
			case AttackResult::Hit: resultDesc = "Hit"; break;
			case AttackResult::Sink:resultDesc = "Sink"; break;
			default: ;
			}

			GameLogger.logFile << "Player " << playerIdToPlayNext << " attack in (" << tempPair.first << "," << tempPair.second << ") result: " << resultDesc << endl;

			//update players - Notify with values 1-10 and not 0-9
			//algo1.algo->notifyOnAttackResult(playerIdToPlayNext, tempPair.first + 1, tempPair.second + 1, tempattackresult);
			//algo2.algo->notifyOnAttackResult(playerIdToPlayNext, tempPair.first + 1, tempPair.second + 1, tempattackresult);

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
					PrintSinkCharRec(mainGameBoard, bonus, tempPair.first, tempPair.second, playerTosetColor);
				}
				else // In case hit update only the target cell
					bonus.PrintPlayerChar(mainGameBoard[tempPair.first][tempPair.second], tempPair.second, tempPair.first, playerTosetColor);
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
			GameLogger.logFile << "Invlaid attack <" << tempPair.first << "," << tempPair.second
								<< "> for player " << playerIdToPlayNext << ". Flipping players" << endl;
			// Flip players
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
		}
	}

	bonus.Dispose(); // Important: Don't touch and don't change the order of statements [Mordehai]
	PrintPoints(playerAboardDetails, playerBboardDetails);*/
	return 0;
}

void GameManager::GameManagerCleanup() const
{
	// Do nothing
}

void GameManager::InitGameManagerLog()
{
	stringstream fileName;
	fileName << "C:\\Temp\\GameLog\\" << this_thread::get_id() << "_" << game_id << ".log";
	GameBoardUtils::InitLogger(GameLogger, fileName.str());

	GameLogger << "Starting Execution thread " << this_thread::get_id() << ". With game id: " << game_id << endl;
	GameLogger << "Board: " << endl;

	mainGameBoard.PrintBoard(GameLogger.logFile);
 }

int GameManager::RunGame()
{
	InitGameManagerLog();

	int code = GameInitializer();
	if(code == ErrorExitCode)
	{
		return code;
	}
	GameLogger.logFile << "===== Game Initilized =======" << endl;
	return 0;
	code = PlayGame();
	
	GameLogger.logFile << "Game exit code is " << code << endl;

	GameManagerCleanup();
	return code;
}