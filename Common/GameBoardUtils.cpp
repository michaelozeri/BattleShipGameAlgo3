#include "GameBoardUtils.h"
#include "IOLib.h"
#include "Contants.h"
#include <thread>
#include <Windows.h>
#include <vector>

void GameBoardUtils::InitBoard(char** board, int rows, int cols, char InitChar = BLANK)
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			board[i][j] = InitChar;
		}
	}
}

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

void GameBoardUtils::LoadLineToBoard(char** board, int row, int cols, const string& cs) {
	char* currentRow = board[row];

	int lenght_int = static_cast<int>(cs.length());
	int len = lenght_int < cols ? lenght_int : cols;
	for (int i = 0; i < len; i++)
	{
		char currentChar = cs[i];
		currentRow[i] = IsLegalBoradChar(currentChar) ? currentChar : BLANK;
	}
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

char** GameBoardUtils::InitializeNewEmptyBoard(int rows, int cols)
{
	char** board = new char*[rows];
	for (int i = 0; i < rows; ++i)
	{
		board[i] = new char[cols];
	}
	return board;
}

void GameBoardUtils::DeleteBoard(char** board, int rows) {
	// Delete board array
	for (int i = 0; i < rows; ++i) {
		delete[] board[i];
	}
	delete[] board;
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

void GameBoardUtils::CloneBoardToPlayer(const char** full_board, int playerID, char** player_board, int rows, int cols,bool CopyAllChars) {
	InitBoard(player_board, rows, cols);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			player_board[i][j] = IsPlayerIdChar(playerID, full_board[i][j]) ? full_board[i][j] : player_board[i][j];
		}
	}
}

void GameBoardUtils::CloneBoard(char** full_board, char** player_board, int rows, int cols) {
	InitBoard(player_board, rows, cols);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			player_board[i][j] = full_board[i][j];
		}
	}
}


/*
* @param argc - of main program
* @param argv - of main program
* @param filesuffix - the suffix of the file we are searching for
* @return path to .sboard file (non-default / "" in case the working directory is chosen )
* @return "ERR" in case of error / file not found
*/
string GameBoardUtils::GetFilePathBySuffix(int argc, string customPath, string filesuffix, bool direxists) //TODO: check this is working after system() remove
{
	char currentdirectory[_MAX_PATH];
	//reference: taken from : http://stackoverflow.com/questions/19691058/c-visual-studio-current-working-directory
	_fullpath(currentdirectory, ".\\", _MAX_PATH); // obtain current directory
	string filename, suffix;
	string delimiter = ".";
	string nondefaultpath = customPath;
	nondefaultpath.erase(std::remove(nondefaultpath.begin(), nondefaultpath.end(), '"'), nondefaultpath.end());
	string systemcallcommand;
	size_t pos;
	string templine;
	char buffer[4096];
	FILE* fp;
	if (argc > 1) {
		if (direxists) {
			fp = _popen(("2>NUL dir /a-d /b "+ nondefaultpath).c_str(), "r");
			while (fgets(buffer, 4095, fp))
			{
				templine = string(buffer);
				pos = templine.find(delimiter);
				suffix = templine.substr(pos, templine.length());
				if((suffix.at(suffix.length()-1))=='\n')
				{
					suffix = suffix.substr(0, suffix.length() - 1);
				}
				filename = templine.substr(0, pos);
				if (!strcmp(suffix.c_str(), filesuffix.c_str())) {
					_pclose(fp);
					if (nondefaultpath.at(nondefaultpath.length() - 1) == '\\') {
						return nondefaultpath + filename + suffix; //argument is inserted with '\' at string ending
					}
					return nondefaultpath + '\\' + filename + suffix; //argument is inserted without '\' at ending
				}
			}
			
			//no more files to read from non - default path
			if (filesuffix == ".sboard") {
				cout << "Missing board file (*.sboard) looking in path: " << currentdirectory << endl;
			}
			else if (filesuffix == ".attack-a") {
				cout << "Missing attack file for player A(*.attack - a) looking in path: " << currentdirectory << endl;
			}
			else {
				cout << "Missing attack file for player B(*.attack - b) looking in path: " << currentdirectory << endl;
			}
			_pclose(fp);
		}
	}
	fp = _popen(("2>NUL dir /a-d /b " + (string)currentdirectory).c_str(), "r");
	while (fgets(buffer, 4095, fp))
	{
		templine = string(buffer);
		pos = templine.find(delimiter);
		suffix = templine.substr(pos, templine.length());
		filename = templine.substr(0, pos);
		if (!strcmp(suffix.c_str(), filesuffix.c_str())) {
			_pclose(fp);
			if (nondefaultpath.at(nondefaultpath.length() - 1) == '\\') {
				return nondefaultpath + filename + suffix; //argument is inserted with '\' at string ending
			}
			return nondefaultpath + '\\' + filename + suffix; //argument is inserted without '\' at ending
		}
	}

	cout << filesuffix + " file was not found in working directory" << endl;
	_pclose(fp);
	return "ERR";
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

char** GameBoardUtils::ClonePlayerBoard(const char** fullBoard, int i, int rows, int cols)
{
	char** playerBoard = GameBoardUtils::InitializeNewEmptyBoard(rows,cols);
	GameBoardUtils::CloneBoardToPlayer(fullBoard, i, playerBoard,rows,cols);
	return playerBoard;
}

void GameBoardUtils::ChangeFontSize()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX orig = { sizeof(orig) };

	if (GetCurrentConsoleFontEx(hStdout, FALSE, &orig))
	{
		//AppLogger.logFile << "Got\n"; 
	}
	else
	{
		//AppLogger.logFile << GetLastError
	}

	orig.dwFontSize.X = 12;
	orig.dwFontSize.Y = 16;

	if (SetCurrentConsoleFontEx(hStdout, FALSE, &orig))
	{
		//AppLogger.logFile << "Set\n";
	}
	else
	{
		//AppLogger.logFile << endl << GetLastError();
	}

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

