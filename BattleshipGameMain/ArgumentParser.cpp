#include "IArgumentParser.h"
#include <vector>
using namespace std;

void IArgumentParser::ParseArguments(Configuration& config, int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			string currentArgumnet = argv[i];
			if (currentArgumnet.substr(0,8).compare("-threads")) {
				config.numberOfThreads = atoi(currentArgumnet.substr(9, currentArgumnet.size - 9).c_str());
			}
			else {
				config.path = currentArgumnet;
			}
		}
	}
	else {
		//TODO: check if needed to put in path null or not
		//number of threads is 4
	}
}
