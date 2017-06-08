#pragma once

#include "Contants.h"
#include <thread>
#include "IOLib.h"
#include "../BattleshipGameMain/GameBoard.h"

using namespace std;

/*
* a utility class for performing static functions at main function
*/
class GameBoardUtils {
public:
	static char*** InitBoard(int rows, int cols,int depth, char InitChar);
	static bool IsPlayerIdChar(int playerID, char current);
	static int GetCharPlayerId(char current);
	static bool IsLegalBoradChar(char current);
	static void LoadLineToBoard(char** board, int row, int cols, const string& cs);
	static void DeleteBoardArray(vector<GameBoard> m_boardArray);
	static GameBoard CreateGameBoardFromFile(const string& filePath);
	static void PrintBoard(ostream& stream, GameBoard& board);
	static void CloneBoardToPlayer(const char** full_board, int playerID, char** player_board, int rows, int cols);
	static void CloneBoard(char** full_board, char** player_board, int rows, int cols);
	static void MarkCannotAttack(char** markBoard, int playernum, const char** mainBoard, int rows, int cols );
	static string GetFilePathBySuffix(int argc, string customPath, string filesuffix, bool direxists = true);
	static bool DirExists(const std::string& dirName_in);
	static char** ClonePlayerBoard(const char** fullBoard, int i, int rows, int cols);
	static void ChangeFontSize();
	static void InitLogger(Logger& logger,string logpath);
	static vector<string> splitLineByDelimiter(string& line, char delimiter);
	static BoardFileErrorCode fillBoardData(char*** board, FileReader& fileReader, int rows, int cols, int depth);
	static BoardFileErrorCode ValidateGameBoard(const GameBoard& gameBoard);
};