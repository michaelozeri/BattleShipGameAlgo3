#include "IArgumentParser.h"
#include "IFileDirectoryUtils.h"

#define DefaultThreadNum 4

bool IArgumentParser::ParseArguments(Configuration& config, int argc, char* argv[])
{
	config.thread_num = DefaultThreadNum;
	config.path = "";
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			string s = argv[i];
			if (!s.compare("-threads"))
			{
				int num = atoi(argv[i + 1]);
				config.thread_num = num > 0 ? num : config.thread_num;
				++i;
			}
			else
			{
				config.path = argv[i];
				//MainLogger << "IArgumentParser Path: " << config.path << endl;
			}
		}
	}

	config.path = GetValidPath(config.path);
	return !config.path.empty();
	//MainLogger << "IArgumentParser: Threads- " << config.thread_num << endl;
}

string IArgumentParser::GetValidPath(string argPath)
{
	string path;
	if (!argPath.empty())
	{
		//MainLogger.logFile << "Path provided in the argument is " << config.path << endl;
		path = IFileDirectoryUtils::GetFullPath(argPath);
		if (IFileDirectoryUtils::DirExists(path))
		{
			//MainLogger.logFile << "Full path directory exist: " << config.path << endl;
			return path;
		}
		//MainLogger.logFile << "Path is not exist " << config.path << endl;
		cout << "Wrong Path: " << path << endl;
		return "";
	}
	//MainLogger.logFile << "Path is not specified in the argument, getting current working directory" << endl;
	path = IFileDirectoryUtils::GetCurrentWorkingDirectory();

	if (IFileDirectoryUtils::DirExists(path))
	{
		//MainLogger.logFile << "Current working directory is " << config.path << endl;
		return path;
	}
	cout << "Wrong Path: " << path << endl;
	return "";
}
