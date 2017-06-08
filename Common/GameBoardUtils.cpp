#include "GameBoardUtils.h"
#include "IOLib.h"
#include "Contants.h"
#include <thread>
#include <Windows.h>
#include <vector>
#include <sstream>


void GetWrongSizeErrMessage(char type, int player)
{
	char playerChar = player == PlayerAID ? 'A' : 'B';
	printf("Wrong size or shape for ship %c for player %c\n", type, playerChar);
}

int GetBoatTypeSizeFromChar (char type)
{
	switch (type)
	{
	case 'B':
	case 'b':
		return RubberBoatW;
	case 'P':
	case 'p':
		return RocketShipW;
	case 'M':
	case 'm':
		return SubmarineW;
	case 'D':
	case 'd':
		return DestroyerW;
	default: return 0;
	}
}

// Recursion function. Return if adjacent exist and mark all current boat with X
bool MarkAllBoat(char** board, int i, int j, char type, int rows, int cols)
{
	if (i < 0 || i >= rows || j < 0 || j >= cols) // Stop recursion condition
	{
		return false;
	}

	char currentCell = board[i][j];
	if (currentCell == BLANK)
		return false;
	if (currentCell == 'X')
		return false;
	if (board[i][j] != type)
	{
		return true; // Adjacent exist - detect another boat of different type
	}

	board[i][j] = 'X'; // Mark current boat
	bool left = MarkAllBoat(board, i, j - 1, type, rows, cols);
	bool right = MarkAllBoat(board, i, j + 1, type, rows, cols);
	bool up = MarkAllBoat(board, i - 1, j, type, rows, cols);
	bool down = MarkAllBoat(board, i + 1, j, type, rows, cols);

	return left || right || up || down;
}

/*
* this function checks the ship size to the right or down as chosen
*direction 1 = check to the right
*direction 0 = check down
*/
bool CheckShipSize(char** board, char direction, int i, int j, char type, int rows,int cols)
{
	int boardTypeSize = GetBoatTypeSizeFromChar(type);
	int sizecheck = boardTypeSize - 1;

	int size = 0; // Will hold the current size of the boat
	
	if (direction) //if checking to the right
	{
		size_t a = j;
		for (size_t k = j; k <= std::fmin(j + sizecheck, cols - 1); k++)
		{
			if (board[i][k] != type) // Check if next cell in right is ok - should be ok for all loop
			{
				return false;
			}
			if(i < rows-1 && board[i+1][k] == type) // check if there is no cell down of same type - should be flase
			{
				return false;
			}
			size++; // Increment boat size in case all is ok
			
			a = k + 1;
		}
		if(a < cols-1 && board[i][a] == type) //check if boat is not larger than it should be from right
		{
			return false;
		}
		return size == boardTypeSize; // Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
	}
	else //direction of check is down
	{
		size_t a = i;
		for (size_t k = i; k <= fmin(i + sizecheck, rows - 1); k++)
		{
			if (board[k][j] != type)// Check if next cell down is ok - should be ok for all loop
			{
				return false;
			}
			if (j < cols - 1 && board[k][j + 1] == type) // check if there is no cell right of same type - should be flase
			{
				return false;
			}
			if (j > 0 && board[k][j - 1] == type) // check if there is no cell left of same type - should be flase
			{
				return false;
			}
			size++; // Increment boat size in case all is ok
			a = k + 1;
		}
		if (a < rows - 1 && board[a][j] == type) //check if boat is not larger than it should be
		{
			return false;
		}
		return size == boardTypeSize; // Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
	}
}

/*
* this function validates the game board and prints by order the errors in the game
*/
BoardFileErrorCode GameBoardUtils::ValidateGameBoard(const GameBoard& gameBoard) //TODO: mordi finish
{ 
	/*
	int playerAboatNum = 0; // Holds valid boat num from player A
	int playerBboatNum = 0; // Holds valid boat num from player B
	
	//bitwise or these values to not go from true -> false
	bool shapeB = false;
	bool shapeP = false;
	bool shapeM = false;
	bool shapeD = false;
	bool shapeb = false;
	bool shapep = false;
	bool shapem = false;
	bool shaped = false;
	bool adjacentErr = false;

	//checking size of ships
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++)
		{
			char cell = board[i][j];
			if (cell != 'X' && cell != BLANK)  //if i didnt check that place already and not BLANK
			{
				bool checkright = CheckShipSize(board, 1, i, j, cell,rows,cols); //check size to the right
				bool checkdown = CheckShipSize(board, 0, i, j, cell, rows, cols); //check size down

				bool isBoatOfValidSize = GetBoatTypeSizeFromChar(cell) == 1 ? checkright & checkdown : checkright^checkdown; //xor because we want only one of them to be ok
				bool adjacent = MarkAllBoat(board, i, j, cell, rows, cols); // Flag which specified if there is another boat of different type or different player that adjacent to the current boat
				adjacentErr = adjacentErr || adjacent;

				if(isBoatOfValidSize) // Update boat number
				{
					if(IsPlayerIdChar(PlayerAID, cell))
					{
						playerAboatNum++; // Add valid ship for player A
					}
					else if(IsPlayerIdChar(PlayerBID, cell))
					{
						playerBboatNum++; // Add valid ship for player B
					}
				}

				else // Update Error ofType Wrong size or shape for ship <char> for player <ID>
				{
					switch (cell)
					{
					case 'B':
						shapeB = true; break;
					case 'b':
						shapeb = true; break;
					case 'P':
						shapeP = true; break;
					case 'p':
						shapep = true; break;
					case 'M':
						shapeM = true; break;
					case 'm':
						shapem = true; break;
					case 'D':
						shapeD = true; break;
					case 'd':
						shaped = true; break;
					default: ;
					}
				}
			}
		}
	}
	
	// Error Printing
	if(shapeB)
	{
		GetWrongSizeErrMessage('B', PlayerAID);
	}
	if(shapeP)
	{
		GetWrongSizeErrMessage('P', PlayerAID);
	}

	if( shapeM )
	{
		GetWrongSizeErrMessage('M', PlayerAID);
	}
	if(shapeD)
	{
		GetWrongSizeErrMessage('D', PlayerAID);
	}
	if( shapeb)
	{
		GetWrongSizeErrMessage('b', PlayerBID);
	}
	if(shapep)
	{
		GetWrongSizeErrMessage('p', PlayerBID);
	}
	if(shapem)
	{
		GetWrongSizeErrMessage('m', PlayerBID);
	}
	if(shaped)
	{
		GetWrongSizeErrMessage('d', PlayerBID);
	}

	// Print num of legal ships Errors
	if(playerAboatNum < LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooFewShipsA << endl;
	}
	if (playerAboatNum > LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooManyShipsA << endl;
	}
	if (playerBboatNum < LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooFewShipsB << endl;
	}
	if (playerBboatNum > LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		cout << TooManyShipsB << endl;
	}
	
	// Print adjacent Error
	if (adjacentErr) 
	{
		cout << AdjacentERR << endl;
	}

	bool isNotLegalBoard = shapeB || shapeP || shapeM || shapeD || shapeb || shapep || shapem || shaped
		|| adjacentErr || playerAboatNum != LEGAL_NUMBER_OF_SHIPS_PER_PLAYER || playerBboatNum != LEGAL_NUMBER_OF_SHIPS_PER_PLAYER;
	return isNotLegalBoard ? BoardFileErrorCode::UnknownError : BoardFileErrorCode::Success;
	*/
}

char*** GameBoardUtils::InitBoard(int rows, int cols,int depth, char InitChar = BLANK)
{
	char*** board;
	for (int i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; j++)
		{
			board[i][j] = new char[depth];
		}
	}

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			for (size_t k = 0; k < depth; k++)
			{
				board[i][j][k] = InitChar;
			}
		}
	}
	return board;
}

bool GameBoardUtils::IsPlayerIdChar(int playerID, char current){
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




GameBoard GameBoardUtils::CreateGameBoardFromFile(const string& filePath) 
{
	int rows, cols, depth;
	string line;
	//parse text file and get rows, cols, depth
	FileReader fileReader(filePath);
	fileReader.ReadLine(line);
	vector<string> sizeVector = splitLineByDelimiter(line, 'x');
	cols = stoi(sizeVector[0]);
	rows = stoi(sizeVector[1]);
	depth = stoi(sizeVector[2]);

	//initialize board
	char*** board = GameBoardUtils::InitBoard(rows, cols, depth);

	//fill board with data
	if (fillBoardData(board, fileReader, rows, cols, depth) != BoardFileErrorCode::Success) {
		//TODO: exit without exit(0);
	}
	
	//initialize GameBoard
	GameBoard gameBoard(board, rows, cols, depth);

	fileReader.CloseFile();
	return gameBoard;
}



void GameBoardUtils::PrintBoard(ostream& stream, GameBoard& gameBoard) 
{
	stream << "****** printing gameBoard *******" << endl;
	for (int k = 0; k < gameBoard.depth; k++)
	{
		stream << endl;
		for (int i = 0; i < gameBoard.rows; i++)
		{
			for (int j = 0; j < gameBoard.cols; j++)
			{
				stream << gameBoard.m_gameboard[i][j][k];
			}
			stream << endl;
		}
		stream << endl;
	}
}



void GameBoardUtils::MarkCannotAttack(char** markBoard, int playernum, const char** mainBoard, int rows, int cols)
{
	char cannotAttckChar = CannotAttck;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (IsPlayerIdChar(playernum, mainBoard[i][j]))
			{
				// mark me
				markBoard[i][j] = cannotAttckChar;

				//mark left of me
				if ((j > 0) && (!IsPlayerIdChar(playernum, mainBoard[i][j - 1])))
				{
					markBoard[i][j - 1] = cannotAttckChar;
				}
				//mark right of me
				if ((j < cols - 1) && (!IsPlayerIdChar(playernum, mainBoard[i][j + 1])))
				{
					markBoard[i][j + 1] = cannotAttckChar;
				}
				//mark above me
				if ((i > 0) && (!IsPlayerIdChar(playernum, mainBoard[i - 1][j])))
				{
					markBoard[i - 1][j] = cannotAttckChar;
				}
				//mark below me
				if ((i < rows - 1) && (!IsPlayerIdChar(playernum, mainBoard[i + 1][j])))
				{
					markBoard[i + 1][j] = cannotAttckChar;
				}
			}
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



void GameBoardUtils::ChangeFontSize()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX orig = { sizeof(orig) };

	if (GetCurrentConsoleFontEx(hStdout, FALSE, &orig))
	{
		//AppLogger.logFile << "Got\n"; //TODO: restore all instances here of applogger
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

/*
this function splits the line into tokens to a vector and returns the vector
*/
vector<string> GameBoardUtils::splitLineByDelimiter(string& line, char delimiter)
{
	vector<string> result;
	stringstream  data(line);
	string line;
	while (getline(data, line, delimiter))
	{
		result.push_back(line);
	}
	return result;
}

/*
this function fills the board with the board file path given
*/
BoardFileErrorCode GameBoardUtils::fillBoardData(char*** board, FileReader& fileReader,int rows,int cols,int depth) //TODO: check errors while reading file
{
	string tempLine;
	fileReader.ReadLine(tempLine);
	for (int k = 0; k < depth; k++)
	{
		for (int i = 0; i < rows; i++)
		{
			fileReader.ReadLine(tempLine);
			for (int j = 0; j < cols; j++)
			{
				board[i][j][k] = tempLine.at(j);
			}
		}
		fileReader.ReadLine(tempLine);
	}
	return BoardFileErrorCode::Success;
}

#pragma region fix_func

char** GameBoardUtils::ClonePlayerBoard(const char** fullBoard, int i, int rows, int cols)
{
	char** playerBoard = GameBoardUtils::InitializeNewEmptyBoard(rows, cols);
	GameBoardUtils::CloneBoardToPlayer(fullBoard, i, playerBoard, rows, cols);
	return playerBoard;
}

void GameBoardUtils::CloneBoardToPlayer(const char** full_board,
	int playerID, char** player_board, int rows, int cols,
	bool CopyAllChars = false) {

	InitBoard(player_board, rows, cols);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if (playerID == 0)
			{
				player_board[i][j] = IsPlayerIdChar(playerID, full_board[i][j]) ? full_board[i][j] : player_board[i][j]
			}
			else
			{

			}

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


void GameBoardUtils::DeleteBoardArray(vector<GameBoard> m_boardArray) {
	// Delete board array
	for (int i = 0; i < rows; ++i) {
		delete[] board[i];
	}
	delete[] board;
}

#pragma endregion




