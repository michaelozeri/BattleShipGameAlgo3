#include "GameManager.h"
#include "IFileDirectoryUtils.h"
#include "Contants.h"
#include "GameBoardUtils.h"
#include "ShipDetailsBoard.h"
#include "Board3D.h"
#include "GameResultInfo.h"

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

Coordinate GameManager::GetNextPlayerAttack(int player_id) const
{
	unique_ptr<IBattleshipGameAlgo>::pointer algo_ptr = player_id == PlayerAID ? algo_ptr1.get() : algo_ptr2.get();
	return algo_ptr->attack();
}

AttackResult GameManager::GetAttackResult(const Coordinate& pair, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB)
{
	return GameBoardUtils::IsPlayerIdChar(PlayerAID, mainGameBoard.m_board[pair.row][pair.col][pair.depth]) ? detailsA.GetAttackResult(pair) : detailsB.GetAttackResult(pair);
}

pair<int, int> GameManager::PrintPoints(ShipDetailsBoard& playerA, ShipDetailsBoard& playerB)
{
	GameLogger << "Points:" << endl;
	GameLogger << "Player A: " << playerB.negativeScore << endl;
	GameLogger << "Player B: " << playerA.negativeScore << endl;
	return{ playerB.negativeScore, playerA.negativeScore };
}

bool GameManager::IsPlayerWon(int currentPlayer, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB)
{
	return currentPlayer == PlayerAID ? detailsB.IsLoose() : detailsA.IsLoose();
}

// Attack pair 1-10
bool GameManager::ValidAttackCor(const Coordinate& pair) const
{
	return pair.row > 0 && pair.row <= mainGameBoard.m_Rows && pair.col > 0 && pair.col <= mainGameBoard.m_Cols
		&& pair.depth > 0 && pair.depth <= mainGameBoard.m_Depth;
}

std::ostream& operator<< (std::ostream & out, Coordinate const& data)
{
	out << "(" << data.row << ", " << data.col << ", " << data.depth << ")";
	return out;
}

GameResultInfo GameManager::PlayGame()
{
	GameLogger << "Starting Play Game " << game_id << endl;
	ShipDetailsBoard playerAboardDetails(mainGameBoard, PlayerAID);
	ShipDetailsBoard playerBboardDetails(mainGameBoard, PlayerBID);

	int playerIdToPlayNext = PlayerAID;

	//main game play
	bool AattacksDone = false;
	bool BattacksDone = false;

	// While not both of players ended their attacks
	while (!AattacksDone || !BattacksDone)
	{
		Coordinate tempPair = GetNextPlayerAttack(playerIdToPlayNext); 
		if (tempPair.row == AttckDoneIndex && (tempPair.col == AttckDoneIndex) && (tempPair.depth == AttckDoneIndex))
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

			GameLogger << "Attak done for player " << playerIdToPlayNext << " Flipping players" << endl;
			// Flip players
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
		}
		else if (ValidAttackCor(tempPair))
		{
			//aligned both axis -1 because main board starts from (0,0)
			tempPair.depth--;
			tempPair.col--;
			tempPair.row--;

			char attckCell = mainGameBoard.m_board[tempPair.row][tempPair.col][tempPair.depth];
			bool isSelfAttack = GameBoardUtils::IsPlayerIdChar(playerIdToPlayNext, attckCell);

			//calculate attack and update mainboard
			AttackResult tempattackresult = GetAttackResult(tempPair, playerAboardDetails, playerBboardDetails);

			string resultDesc;
			switch (tempattackresult)
			{
			case AttackResult::Miss: resultDesc = "Miss";  break;
			case AttackResult::Hit: resultDesc = "Hit"; break;
			case AttackResult::Sink:resultDesc = "Sink"; break;
			}

			GameLogger.logFile << "Player " << playerIdToPlayNext << " attack in " << tempPair <<" result: " << resultDesc << endl;

			//update players - Notify with values 1-10 and not 0-9. Scope to delete cor object in the end
			{
				Coordinate cor(tempPair.row + 1, tempPair.col + 1, tempPair.depth + 1);
				algo_ptr1->notifyOnAttackResult(playerIdToPlayNext, cor, tempattackresult);
				algo_ptr2->notifyOnAttackResult(playerIdToPlayNext, cor, tempattackresult);
			}

			if (tempattackresult == AttackResult::Miss || isSelfAttack)
			{
				// Flip Players
				playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
			}

			if (IsPlayerWon(PlayerAID, playerAboardDetails, playerBboardDetails))
			{
				GameLogger << "Player A won" << endl;
				pair<int, int> points = PrintPoints(playerAboardDetails, playerBboardDetails);
				return GameResultInfo(PlayerAWinner, points.first, points.second);
			}
			if (IsPlayerWon(PlayerBID, playerAboardDetails, playerBboardDetails))
			{
				GameLogger << "Player B won" << endl;
				pair<int, int> points = PrintPoints(playerAboardDetails, playerBboardDetails);
				return GameResultInfo(PlayerBWinner, points.first, points.second);

			}
		}
		else
		{
			GameLogger << "Invlaid attack " << tempPair << " for player " << playerIdToPlayNext << ". Flipping players" << endl;
			// Flip players in case of invalid attack
			playerIdToPlayNext = (playerIdToPlayNext == PlayerAID) ? PlayerBID : PlayerAID;
		}
	}
	pair<int,int> points = PrintPoints(playerAboardDetails, playerBboardDetails);
	return GameResultInfo(NoWinner, points.first, points.second);
}

void GameManager::InitGameManagerLog()
{
	stringstream fileName;
	fileName << "C:\\Temp\\Foo1\\" << "GameManager_" << this_thread::get_id() << "_" << game_id << ".log";
	GameBoardUtils::InitLogger(GameLogger, fileName.str());

	GameLogger << "Starting Execution thread " << this_thread::get_id() << ". With game id: " << game_id << endl;
	GameLogger << "Board: " << endl;

	mainGameBoard.PrintBoard(GameLogger.logFile);
 }

GameResultInfo GameManager::RunGame()
{
	InitGameManagerLog();

	int code = GameInitializer();
	if (code == ErrorExitCode)
	{
		return GameResultInfo(NoWinner, 0, 0);
	}
	GameLogger.logFile << "===== Game Initilized =======" << endl;

	GameResultInfo resultInfo = PlayGame();

	GameLogger << "Winner: " << resultInfo.Winner << endl;

	GameLogger.logFile << "Game exit code is " << code << endl;
	return resultInfo;
}
