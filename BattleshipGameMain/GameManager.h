#pragma once
#include "IArgumentParser.h"
#include "DllAlgo.h"
#include "ShipDetailsBoard.h"
#include "GameBoard.h"

extern Logger MainLogger;

class GameManager
{
	Configuration& config;
	pair<string, string> m_dllPaths;
	
	bool ConfigureDlls();
	bool ConfigurePath() const;
	bool ConfigureBoards() ;
	bool ConfigureFiles();
	bool InitPlayers();
	bool InitDllAlgo(DllAlgo& algo, const string& path, int playerID) const;

	int GameInitializer();
	static Coordinate GetNextPlayerAttack(int player_id, IBattleshipGameAlgo* player_a, IBattleshipGameAlgo* player_b);
	static AttackResult GetAttackResult(const pair<int, int>& pair, char** board, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB);
	static void PrintPoints(ShipDetailsBoard& playerA, ShipDetailsBoard& playerB);
	static void PrintSinkCharRec(char** maingameboard, Bonus& b, int i, int j, int player);
	static bool IsPlayerWon(int currentPlayer, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB);
	static bool ValidAttackCor(const pair<int, int>& pair);
	static GameBoard* InitializeGameBoardArray(int size);
	int PlayGame() const;

	void GameManagerCleanup() const;
public:
	vector<DllAlgo> m_algoArray;
	vector<GameBoard> m_boardArray;
	GameManager(Configuration& config);
	int RunGame();
	void Test_GetAllAttacks() const;
};
