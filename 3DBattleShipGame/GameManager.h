#pragma once
#include "ShipDetailsBoard.h"
#include "Board3D.h"
#include <memory>
#include "IBattleshipGameAlgo.h"
#include "BoardDataView.h"
#include "GameResultInfo.h"

extern Logger MainLogger;

class GameManager
{
private:
	BoardDataView view1;
	BoardDataView view2;

	int game_id;
	Logger GameLogger;

	unique_ptr<IBattleshipGameAlgo> algo_ptr1;
	unique_ptr<IBattleshipGameAlgo> algo_ptr2;

	// board - will save updated and full board of two players
	Board3D mainGameBoard;

	bool InitPlayers();
	bool InitDllAlgo(int playerID);

	int GameInitializer();
	Coordinate GetNextPlayerAttack(int player_id) const;
	AttackResult GetAttackResult(const Coordinate& pair, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB);
	pair<int,int> PrintPoints(ShipDetailsBoard& playerA, ShipDetailsBoard& playerB);
	static bool IsPlayerWon(int currentPlayer, ShipDetailsBoard& detailsA, ShipDetailsBoard& detailsB);
	bool ValidAttackCor(const Coordinate& pair) const;
	GameResultInfo PlayGame();

	void InitGameManagerLog();
public:
	GameManager(unique_ptr<IBattleshipGameAlgo> algo1, unique_ptr<IBattleshipGameAlgo> algo2, Board3D board, int id);
	GameResultInfo RunGame();
};
