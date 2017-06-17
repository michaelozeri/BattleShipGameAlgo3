#include "3DBoardLoader.h"
#include <sstream>
#include "GameBoardUtils.h"
#include "BoardValidator.h"

BoardLoader::BoardLoader(vector<Board3D>& boardList, string& dirContainerPth): lastIndex(0), boardContainerPath(dirContainerPth)
{
	list = &boardList;
}

void BoardLoader::LoadAllBoards()
{
	bool result = LoadAllAvailableBoardsPath();
	if(!result)
	{
		MainLogger.logFile << "Failed to LoadAllAvailableBoardsPath" << endl;
	}

	int succeded = 0;
	for each(const string& path in boardPathsList)
	{
		result = LoadBoardFromPath(path);
		if(!result)
		{
			MainLogger.logFile << "Failed to load board from path: " << path << endl;
		}
		succeded += result ? 1 : 0;
	}

	MainLogger.logFile << "Succeded to load " << succeded << " boards" << endl;
}

bool BoardLoader::LoadAllAvailableBoardsPath()
{
	// Check if main container folder exist
	if (!IFileDirectoryUtils::DirExists(boardContainerPath))
	{
		MainLogger.logFile << "Path is not exist " << boardContainerPath << endl;
		cout << "Wrong Path: " << boardContainerPath << endl;
		return false;
	}
	MainLogger.logFile << "Full path directory exist: " << boardContainerPath << endl;

	int cnt = IFileDirectoryUtils::GetAllFiles(boardContainerPath, "*.sboard", boardPathsList);
	if (cnt <= 0)
	{
		MainLogger.logFile << "Missing *.sboard file" << endl;
		cout << "Missing board file (*.sboard) looking in path: " << boardContainerPath << endl;
		return false;
	}
	return true;
}

bool BoardLoader::LoadBoardFromPath(const string& path) const
{
	MainLogger.logFile << "Starting to load " << path << endl;

	FileReader file_reader(path);
	BoardDimension dimension;

	bool result = GetBoardDimension(dimension, file_reader);
	if(!result)
	{
		MainLogger.logFile << "Failed to GetBoardDimension" << endl;
		return false;
	}

	// Expected empty line after 
	result = ReadEmptyLineAfterDimension(file_reader);
	if(!result)
	{
		MainLogger.logFile << "Failed to ReadEmptyLineAfterDimension" << endl;
		return false;
	}

	Board3D board3_d(dimension.Rows, dimension.Cols, dimension.Depth);

	MainLogger.logFile << "Loading board" << endl;
	LoadBoardFromFile(board3_d, file_reader);
	board3_d.PrintBoard(MainLogger.logFile);

	MainLogger.logFile << "Starting validation process for 3D game board" << endl;
	result = ValidateBoard(board3_d);

	MainLogger.logFile << "Validation result is: " << result << endl;
	if(result)
	{
		// From this point board3_d is no longer exist
		list->push_back(move(board3_d));
	}
	return result;
}

bool BoardLoader::ValidateBoard(Board3D& boardToValidate)
{
	return BoardValidator::ValidateBoard(boardToValidate, MainLogger);
}

void BoardLoader::LoadBoardFromFile(Board3D& board3_d, FileReader& file_reader) const
{
	int depthReaded = 0;

	while(depthReaded < board3_d.m_Depth && !file_reader.IsEof())
	{
		int rows = 0;
		string line = "";
		file_reader.ReadLine(line);
		while (!line.empty())
		{
			if(rows < board3_d.m_Rows)
			{
				GameBoardUtils::LoadLineToBoard(board3_d.m_board, rows, depthReaded, board3_d.m_Cols, line);
			}

			rows++;
			line = "";
			file_reader.ReadLine(line);
		}
		depthReaded++;
	}
}

bool BoardLoader::ReadEmptyLineAfterDimension(FileReader& file_reader)
{
	if (file_reader.IsEof())
	{
		MainLogger.logFile << "Detect Eof - failed to load board" << endl;
		return false;
	}

	// Expected empty line after 
	string line;
	line = file_reader.ReadLine(line);
	if (!line.empty())
	{
		MainLogger.logFile << "Missing empty line after dimension line " << endl;
		return false;
	}
	return true;
}

bool BoardLoader::GetBoardDimension(BoardDimension& board_dimension, FileReader& file_reader) const
{
	if (file_reader.IsEof())
	{
		MainLogger.logFile << "Detect Eof - failed to load board" << endl;
		return false;
	}

	// Read line and convert to lower case - Enable handle 'X' or 'x'
	string line;
	line = file_reader.ReadLine(line);
	transform(line.begin(), line.end(), line.begin(), tolower);

	// Create new stream
	stringstream  data(line);
	string dim;
	int numOfDim = 0;
	while (getline(data, dim, DimensionDelimiter) && numOfDim < 3)
	{
		int tempDim = ConvertStringToIntSafely(dim);
		if (tempDim <= 0)
		{
			MainLogger.logFile << "Detect invalid dimension " << tempDim << "Failed to load board" << endl;
			return false;
		}

		// Assign relevant Dim
		numOfDim++;
		switch (numOfDim)
		{
		case 1:
			board_dimension.Cols = tempDim;
			break;
		case 2:
			board_dimension.Rows = tempDim;
			break;
		case 3:
			board_dimension.Depth = tempDim;
			break;
		default:;
		}
	}
	if (numOfDim != 3)
	{
		MainLogger.logFile << "failed To find 3 dimension. Found only " << numOfDim << ". Failed to load board" << endl;
		return false;
	}

	MainLogger.logFile << "Succeded to load dimensions:" << board_dimension.Cols << "x" << board_dimension.Rows << "x" << board_dimension.Depth << endl;
	return true;
}

int BoardLoader::ConvertStringToIntSafely(string& line) const
{
	try
	{
		int number = stoi(line);
		return number;
	}
	catch (const std::exception&)
	{
		return -1;
	}
}

