#include "IArgumentParser.h"
#include "IFileDirectoryUtils.h"

#define DefaultThreadNum 4

void IArgumentParser::ParseArguments(Configuration& config, int argc, char* argv[])
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
				MainLogger << "IArgumentParser Path: " << config.path << endl;
			}
		}
	}
	MainLogger << "IArgumentParser: Threads- " << config.thread_num << endl;
}