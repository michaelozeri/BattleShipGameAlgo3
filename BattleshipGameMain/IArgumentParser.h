#pragma once
#include "../Common/Bonus.h"

using namespace std;
class Configuration
{
public:
	string path;
	int numberOfThreads = 4;
	BonusParams bonusParam;
};

class IArgumentParser
{
public :
	static void ParseArguments(Configuration& config, int argc, char* argv[]);
};