#pragma once
#include "DllAlgo.h"
#include "ShipDetailsBoard.h"
#include "Board3D.h"
#include <memory>

extern Logger MainLogger;

class GameManager
{
private:
	int game_id;
	Logger GameLogger;
	IBattleshipGameAlgo* algo1;
	IBattleshipGameAlgo* algo2;

	unique_ptr<IBattleshipGameAlgo> ptr1;
	unique_ptr<IBattleshipGameAlgo> ptr2;

	// board - will save updated and full board of two players
	Board3D mainGameBoard;

	bool InitPlayers();
	bool InitDllAlgo(DllAlgo& algo, const string& path, int playerID) const;

	int GameInitializer();
	pair<int, int> GetNextPlayerAttack(int player_id, IBattleshipGameAlgo* player_a, IBattleshipGameAlgo* player_b);
	AttackResult GetAttackResult(const pair<int, int>& pair, char** board, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB);
	void PrintPoints(ShipDetailsBoard& playerA, ShipDetailsBoard& playerB);
	//void PrintSinkCharRec(char** maingameboard, Bonus& b, int i, int j, int player);
	//void PrintSinkCharRec(char** maingameboard, Bonus& b, int i, int j, int player);
	bool IsPlayerWon(int currentPlayer, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB);
	bool ValidAttackCor(const pair<int, int>& pair);
	int PlayGame() const;

	void GameManagerCleanup() const;
	void InitGameManagerLog();
public:
	GameManager(unique_ptr<IBattleshipGameAlgo> algo1, unique_ptr<IBattleshipGameAlgo> algo2, Board3D board, int id);

	int RunGame();
};
