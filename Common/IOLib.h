#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

class FileReader
{
	string filePath;
	ifstream fin;
	bool isOpen;
public:
	FileReader(const std::string& path) : filePath(path), isOpen(false)
	{
		fin.open(path);
		isOpen = true;
	}

	string& ReadLine(string& line)
	{
		if (!IsEof())
		{
			getline(fin, line);
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());
			return line;
		}
		return line;
	}

	bool IsEof() const
	{
		return fin.eof();
	}

	void PrintFile()
	{
		string l;
		while (getline(fin, l))
		{
			cout << l << endl;
		}
	}

	void CloseFile()
	{
		fin.close();
		isOpen = false;
	}

	~FileReader ()
	{
		if (isOpen)
			CloseFile();
	}
};

/*
* a logger for the program. will write all the outputs to a log file for more easy debugging
*/
class Logger{
public:
	ofstream logFile;

	void InitLogger(const std::string& logPath)
	{
		logFile.open(logPath);
	}
	void LoggerDispose()
	{
		logFile << "Disposing log file" << endl;

		logFile.close();
	}
};

