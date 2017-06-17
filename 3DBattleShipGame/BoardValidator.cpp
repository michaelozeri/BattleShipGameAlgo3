#include "BoardValidator.h"
#include "IFileDirectoryUtils.h"

bool IsPlayerIdChar(int playerID, char current) {
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

/*
* this function gets a boat type by its representing char and
* returns the boat size to be excpected
*/
int GetBoatTypeSizeFromChar(char type)
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

/*
* this function checks the ship size to the right or down as chosen
*direction 2 = check to depth
*direction 1 = check to the right
*direction 0 = check down
*/
bool CheckShipSize(vector<vector<vector<char>>>& board, int direction, int i, int j, int k, char charType, int rows, int cols, int depth)
{
	int boardTypeSize = GetBoatTypeSizeFromChar(charType);
	int sizecheck = boardTypeSize - 1;

	int boatSizeCounted = 0; // Will hold the current size of the boat

	if (direction == 1) //if checking to the right
	{
		int a = j;
		for (int l = j; l <= std::fmin(j + sizecheck, cols - 1); l++) //iterate until end of ship or end of line
		{
			if (board[i][l][k] != charType) // Check if next cell in right is ok - should be ok for all loop
			{
				return false;
			}
			if (i < rows - 1 && board[i + 1][l][k] == charType) // check if there is no cell down of same type - should be flase
			{
				return false;
			}
			if (k < depth - 1 && board[i][l][k + 1] == charType) // check if there is no cell in depth of same type - should be flase
			{
				return false;
			}
			boatSizeCounted++; // Increment boat size in case all is ok
							   //set a for checking if boat is not oversized
			a = l + 1;
		}
		if (a < cols - 1 && board[i][a][k] == charType) //check if boat is not larger than it should be from right
		{
			return false;
		}
		// Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
		return (boatSizeCounted == boardTypeSize);
	}

	else if (direction == 0) //if checking down
	{
		int a = i;
		for (int l = i; l <= fmin(i + sizecheck, rows - 1); l++)
		{
			if (board[l][j][k] != charType)// Check if next cell down is ok - should be ok for all loop
			{
				return false;
			}
			if (j < cols - 1 && board[l][j + 1][k] == charType) // check if there is no cell right of same type - should be flase
			{
				return false;
			}
			if (j > 0 && board[l][j - 1][k] == charType) // check if there is no cell left of same type - should be flase
			{
				return false;
			}
			if (k < depth - 1 && board[l][j][k + 1] == charType) // check if there is no cell in depth of same type - should be flase
			{
				return false;
			}
			boatSizeCounted++; // Increment boat size in case all is ok
			a = l + 1;
		}
		if (a < rows - 1 && board[a][j][k] == charType) //check if boat is not larger than it should be
		{
			return false;
		}
		// Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
		return (boatSizeCounted == boardTypeSize);
	}
	else { //checking depth
		int a = k;
		for (int l = k; l <= fmin(k + sizecheck, depth - 1); l++)
		{
			if (board[i][j][l] != charType)// Check if next cell in depth is ok - should be ok for all loop
			{
				return false;
			}
			if (l > k) { //checking all around squares when going up in depth
				if (i < rows - 1) {
					if (board[i + 1][j][l] == charType) {
						return false;
					}
				}
				if (i > 0) {
					if (board[i - 1][j][l] == charType) {
						return false;
					}
				}
				if (j < cols - 1) {
					if (board[i][j + 1][l] == charType) {
						return false;
					}
				}
				if (j > 0) {
					if (board[i][j - 1][l] == charType) {
						return false;
					}
				}
			}
			if (l == k) { //checking down and right one square
				if (i < rows - 1) {
					if (board[i + 1][j][l] == charType) {
						return false;
					}
				}
				if (j < cols - 1) {
					if (board[i][j + 1][l] == charType) {
						return false;
					}
				}
			}
			boatSizeCounted++; // Increment boat size in case all is ok
			a = l + 1;
		}
		if (a < depth - 1 && board[i][j][a] == charType) //check if boat is not larger than it should be
		{
			return false;
		}
		// Return true in case size is good, is smaller return false (if greater than terminates in the above statment)
		return (boatSizeCounted == boardTypeSize);
	}
}

/*
* Recursive function. Return if adjacent exist and mark all current boat with 'X'
*/
bool MarkAllBoat(vector<vector<vector<char>>>& board, int i, int j, int k, char type, int rows, int cols, int depth)
{
	// Stop recursion condition
	if (i < 0 || i >= rows || j < 0 || j >= cols || k<0 || k >= depth)
	{
		return false;
	}

	char currentCell = board[i][j][k];

	if (currentCell == BLANK)
		return false;
	if (currentCell == 'X')
		return false;
	if (board[i][j][k] != type)
	{
		return true; // Adjacent exist - detect another boat of different type
	}

	board[i][j][k] = 'X'; // Mark current boat
	bool left = MarkAllBoat(board, i, j - 1, k, type, rows, cols, depth);
	bool right = MarkAllBoat(board, i, j + 1, k, type, rows, cols, depth);
	bool up = MarkAllBoat(board, i - 1, j, k, type, rows, cols, depth);
	bool down = MarkAllBoat(board, i + 1, j, k, type, rows, cols, depth);
	bool upDepth = MarkAllBoat(board, i, j, k + 1, type, rows, cols, depth);
	bool downDepth = MarkAllBoat(board, i, j, k - 1, type, rows, cols, depth);

	return left || right || up || down || upDepth || downDepth;
}

void GetWrongSizeErrMessage(char type, int player)
{
	char playerChar = player == PlayerAID ? 'A' : 'B';
	MainLogger << "Wrong size or shape for ship " << type << " for player " << playerChar << endl;
}

bool BoardValidator::ValidateBoard(Board3D boardToValidate, Logger& mainLogger)
{
	int playerAboatNum = 0; // Holds valid boat num from player A
	int playerBboatNum = 0; // Holds valid boat num from player B
	int rows = boardToValidate.m_Rows;
	int cols = boardToValidate.m_Cols;
	int depth = boardToValidate.m_Depth;

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
			for (int k = 0; k < depth; k++)
			{
				char cellChar = boardToValidate.m_board[i][j][k];
				if (cellChar != 'X' && cellChar != BLANK)  //if i didnt check that place already and not BLANK
				{
					bool checkdepth = CheckShipSize(boardToValidate.m_board, 2, i, j, k, cellChar, rows, cols, depth); //check size in direction of depth
					bool checkright = CheckShipSize(boardToValidate.m_board, 1, i, j, k, cellChar, rows, cols, depth); //check size to the right
					bool checkdown = CheckShipSize(boardToValidate.m_board, 0, i, j, k, cellChar, rows, cols, depth); //check size down

					int checkSum = checkdown + checkdepth + checkright; //checkSum because we want one to be ok
					if (checkSum > 1) {
						checkSum = 0;
					}

					int isBoatOfValidSize = GetBoatTypeSizeFromChar(cellChar) == 1 ? checkright & checkdown & checkdepth : checkSum; 

					// Flag which specified if there is another boat of different type or different player that adjacent to the current boat
					bool adjacent = MarkAllBoat(boardToValidate.m_board, i, j,k, cellChar, rows, cols,depth); 
					adjacentErr = adjacentErr || adjacent;

					if (isBoatOfValidSize) // Update boat number
					{
						if (IsPlayerIdChar(PlayerAID, cellChar))
						{
							playerAboatNum++; // Add valid ship for player A
						}
						else if (IsPlayerIdChar(PlayerBID, cellChar))
						{
							playerBboatNum++; // Add valid ship for player B
						}
					}

					else // Update Error ofType Wrong size or shape for ship <char> for player <ID>
					{
						switch (cellChar)
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
						default:;
						}
					}
				}
			}
		}
	}

	// Error Printing
	if (shapeB) 
	{
		GetWrongSizeErrMessage('B', PlayerAID);
	}
	if (shapeP)
	{
		GetWrongSizeErrMessage('P', PlayerAID);
	}

	if (shapeM)
	{
		GetWrongSizeErrMessage('M', PlayerAID);
	}
	if (shapeD)
	{
		GetWrongSizeErrMessage('D', PlayerAID);
	}
	if (shapeb)
	{
		GetWrongSizeErrMessage('b', PlayerBID);
	}
	if (shapep)
	{
		GetWrongSizeErrMessage('p', PlayerBID);
	}
	if (shapem)
	{
		GetWrongSizeErrMessage('m', PlayerBID);
	}
	if (shaped)
	{
		GetWrongSizeErrMessage('d', PlayerBID);
	}

	// Print num of legal ships Errors
	if (playerAboatNum < LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		MainLogger << TooFewShipsA << endl;
	}
	if (playerAboatNum > LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		MainLogger << TooManyShipsA << endl;
	}
	if (playerBboatNum < LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		MainLogger << TooFewShipsB << endl;
	}
	if (playerBboatNum > LEGAL_NUMBER_OF_SHIPS_PER_PLAYER)
	{
		MainLogger << TooManyShipsB << endl;
	}

	// Print adjacent Error
	if (adjacentErr)
	{
		MainLogger << AdjacentERR << endl;
	}

	// Checking number of boat
	bool numBoatsPlayerA = playerAboatNum != LEGAL_NUMBER_OF_SHIPS_PER_PLAYER; // true if wrong number
	if (numBoatsPlayerA)
		MainLogger << "Number of boats for player A is " << playerAboatNum << " instead of " << LEGAL_NUMBER_OF_SHIPS_PER_PLAYER << endl;

	bool numBoatsPlayerB = playerBboatNum != LEGAL_NUMBER_OF_SHIPS_PER_PLAYER; // true if wrong number
	if (numBoatsPlayerB)
		MainLogger << "Number of boats for player B is " << playerBboatNum << " instead of " << LEGAL_NUMBER_OF_SHIPS_PER_PLAYER << endl;

	bool isNotLegalBoard = shapeB || shapeP || shapeM || shapeD || shapeb || shapep || shapem || shaped
		|| adjacentErr || numBoatsPlayerA || numBoatsPlayerB;
	return !isNotLegalBoard;
} 