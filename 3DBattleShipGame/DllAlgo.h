#pragma once
#include "IBattleshipGameAlgo.h"
#include "IOLib.h"
#include "Windows.h"

using namespace std;

extern Logger MainLogger;

typedef IBattleshipGameAlgo *(*GetAlgorithmFuncType)();
extern GetAlgorithmFuncType getPlayerAlgo;

class DllAlgo
{
public:
	DllAlgo();
	IBattleshipGameAlgo* algo;
	bool LoadDll(const string& dllPath, string dllName);
	IBattleshipGameAlgo* GetAlgoInstance() const;
	void Dispose() const;
	string path;
	string DllName;
private:
	
	pair <HINSTANCE, GetAlgorithmFuncType> handle;
};
