#pragma once
#include "IOLib.h"
#include "IFileDirectoryUtils.h"
#include "Board3D.h"

extern Logger MainLogger;

struct BoardDimension
{
public:
	int Rows;
	int Cols;
	int Depth;
};

class BoardLoader
{
public:
	BoardLoader(vector<Board3D>& boardList, string& dirContainerPth);
	void LoadAllBoards();

private:
	vector<Board3D>*  list;
	int lastIndex;
	vector<string> boardPathsList;
	string& boardContainerPath;
	IFileDirectoryUtils file_directory;

	bool LoadAllAvailableBoardsPath();
	bool LoadBoardFromPath(const string& path) const;
	static bool ValidateBoard(Board3D& boardToValidate);
	void LoadBoardFromFile(Board3D& board3_d, FileReader& file_reader) const;
	static bool ReadEmptyLineAfterDimension(FileReader& file_reader);
	bool GetBoardDimension(BoardDimension& board_dimension, FileReader& file_reader) const;
	int ConvertStringToIntSafely(string& line) const;
};

