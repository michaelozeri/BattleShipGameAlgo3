#include "GameBoardUtils.h"
#include "IOLib.h"
#include "Contants.h"
#include <thread>
#include <Windows.h>
#include <vector>



int GameBoardUtils::GetCharPlayerId(char current)
{
	if (IsPlayerIdChar(PlayerAID, current))
	{
		return PlayerAID;
	}
	else if (IsPlayerIdChar(PlayerBID, current) )
		return PlayerBID;

	return 2;
}

bool GameBoardUtils::IsLegalBoradChar(char current) {
	return IsPlayerIdChar(PlayerAID, current) || IsPlayerIdChar(PlayerBID, current);
}



void GameBoardUtils::LoadLineToBoard(vector<vector<vector<char>>>& borad3D, int row, int depth, int numOfcols, const string& cs)
{
	int lenght_int = static_cast<int>(cs.length());
	int len = lenght_int < numOfcols ? lenght_int : numOfcols;
	for (int i = 0; i < len; i++)
	{
		char currentChar = cs[i];
		borad3D[row][i][depth] = IsLegalBoradChar(currentChar) ? currentChar : BLANK;
	}
}


void GameBoardUtils::PrintBoard(ostream& stream, vector<vector<vector<char>>> board, int rows, int cols,int depth) 
{
	for (int k = 0; k < depth; k++)
	{
		stream << endl;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				stream << board[i][j][k];
			}
			stream << endl;
		}
		stream << endl;
	}
	
}



//taken from: http://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
bool GameBoardUtils::DirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

void GameBoardUtils::InitLogger(Logger& logger,string logpath)
{
	logger.InitLogger(logpath);
}

bool GameBoardUtils::IsPlayerIdChar(int playerID, char current) 
{
	if (playerID == PlayerAID)
	{
		return current == RubberBoatA ||
			current == RocketShipA ||
			current == SubmarineA ||
			current == DestroyerA;
	}
	if (playerID == PlayerBID)
	{
		return  current == RubberBoatB ||
			current == RocketShipB ||
			current == SubmarineB ||
			current == DestroyerB;
	}
	return false;
}

