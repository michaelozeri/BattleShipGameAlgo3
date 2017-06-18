#pragma once
#include <string>

using namespace std;
class Configuration
{
public:
	string path;
	int thread_num;
};

class IArgumentParser
{
public :
	static bool ParseArguments(Configuration& config, int argc, char* argv[]);
	static string GetValidPath(string argPath);
};