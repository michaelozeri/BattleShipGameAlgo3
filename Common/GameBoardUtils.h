#pragma once

#include "IOLib.h"
#include <vector>

using namespace std;

/*
* a utility class for performing static functions at main function
*/
class GameBoardUtils {
public:
	
	static bool IsPlayerIdChar(int playerID, char current);
	static int GetCharPlayerId(char current);
	static bool IsLegalBoradChar(char current);
	static void LoadLineToBoard(vector<vector<vector<char>>>& borad3D, int row, int depth, int numOfcols, const string& cs);
	static void PrintBoard(ostream& stream, vector<vector<vector<char>>> board, int rows, int cols, int depth);
	static bool DirExists(const std::string& dirName_in);
	static void InitLogger(Logger& logger,string logpath);
};