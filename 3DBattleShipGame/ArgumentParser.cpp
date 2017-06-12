#include "IArgumentParser.h"

#define DefaultThreadNum 4

void IArgumentParser::ParseArguments(Configuration& config, int argc, char* argv[])
{
	if (argc > 1)
	{
		config.thread_num = DefaultThreadNum;
		for (int i = 1; i < argc; i++)
		{
			string s = argv[i];
			if (!s.compare("-threads"))
			{
				int num = atoi(argv[i + 1]);
				config.thread_num = num > 0 ? num : config.thread_num;
				i++;
			}
			else
			{
				config.path = argv[i];
			}
		}
	}
}