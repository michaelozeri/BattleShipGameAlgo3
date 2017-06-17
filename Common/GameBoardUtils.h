#pragma once

#include "IOLib.h"
#include <vector>

using namespace std;

/*
* a utility class for performing static functions at main function
*/
class GameBoardUtils {
public:
	static void InitBoard(char** board, int rows, int cols, char InitChar);
	static bool IsPlayerIdChar(int playerID, char current);
	static int GetCharPlayerId(char current);
	static bool IsLegalBoradChar(char current);
	static void LoadLineToBoard(char** board, int row, int cols, const string& cs);
	static void LoadLineToBoard(vector<vector<vector<char>>>& borad3D, int row, int depth, int numOfcols, const string& cs);
	static char** InitializeNewEmptyBoard(int rows, int cols);
	static void DeleteBoard(char** board, int rows);
	static void PrintBoard(ostream& stream, vector<vector<vector<char>>> board, int rows, int cols, int depth);
	static void CloneBoardToPlayer(const char** full_board, int playerID, char** player_board, int rows, int cols,bool copyAllChars = false);
	static void CloneBoard(char** full_board, char** player_board, int rows, int cols);
	static bool DirExists(const std::string& dirName_in);
	static char** ClonePlayerBoard(const char** fullBoard, int i, int rows, int cols);
	static void ChangeFontSize();
	static void InitLogger(Logger& logger,string logpath);
};