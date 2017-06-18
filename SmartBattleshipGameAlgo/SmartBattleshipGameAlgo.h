#pragma once
#include <vector>
#include <tuple>
#include "IBattleshipGameAlgo.h"
#include "Board3D.h"

using namespace std;

enum class AttackMode {RandomMode, TargetMode};
enum class AttackDir {rows_dir, cols_dir, depth_dir, unknown};

/*
* a class that represents a player at the game
* holds the current information of the player while playing the game
*/
class SmartBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	//IBattleshipGameAlgo - override methods
	void setBoard(const BoardData& board) override; // called once to notify player on his board
	Coordinate attack() override; // ask player for his move
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override; // notify on last move result
	void setPlayer(int player) override;
	
	SmartBattleshipGameAlgo();

	static Coordinate AllignCord(Coordinate currCoor, bool isNegative = false);
	Coordinate GetValidRandomAttack();
	Coordinate GetValidOptionalAttack();
	void AddSqureCellsToQueue(int row, int col, int depth);
	void AddPotentialAttckIfLegal(int row, int col, int depth, AttackDir dir);
	void HandleMyRandomMode(int row, int col, int depth, AttackResult result);
	void HandleMyTargetMode(int row, int col, int depth, AttackResult result);
	tuple<int, int, int, AttackDir> Dequeue(int row, int col, int depth);
	void AddToQueueAfterHit(int row, int col, int depth);
	void HandleMyAttackResult(int row, int col, int depth, AttackResult result);
	void HandleRivalAttackResult(int row, int col, int depth, AttackResult result);
	void MarkInvalidCell(int row, int col, int depth, AttackResult result); 
	void MarkSinkBattleAroundAsInvlid(int row, int col, int depth);
	
	

private:
	// Board3D of the smart algo
	Board3D m_Board3d;
	
	// Number rows - set in SetBoard func
	int m_numRows;

	// Number cols - set in SetBoard func
	int m_numCols;
	
	// Depth - set in SetBoard func
	int m_depth;

	// Attack Mode
	AttackMode m_mode;

	// Current Direction attack
	AttackDir m_currDir;

	//My player number
	int m_myPlayerNum;
	
	// Holds the valid attack for generating a random attack
	vector<Coordinate> m_attacksRemain;

	// Holds potential attacks after mode is Target <row,col,dir(Vertical/Horizontal)>
	vector<tuple<int, int, int, AttackDir>> m_PotentialAttacks;

	// some private methods.
	static int GetRandom(size_t maxNumber);

	void StartRandomAttackMode();

	bool IsAttackValid(int row, int col, int depth) const;
};