#include "DllAlgo.h"

DllAlgo::DllAlgo(): algo(nullptr)
{
}

bool DllAlgo::LoadDll(const string& dllPath, string dllName)
{
	path = string(dllPath);
	DllName = dllName;

	// Load dynamic library
	HINSTANCE hDll = LoadLibraryA(path.c_str()); // Notice: Unicode compatible version of LoadLibrary
	if (!hDll)
	{
		MainLogger.logFile << "Could not load the dynamic library " << path << endl;
		return false;
	}

	GetAlgorithmFuncType GetAlgorithmFunc = reinterpret_cast<GetAlgorithmFuncType>(GetProcAddress(hDll, "GetAlgorithm"));
	if (GetAlgorithmFunc == nullptr)
	{
		MainLogger.logFile << "could not load function GetAlgorithm() ErrorCode: " << GetLastError() << endl;
		return false;
	}

	handle = { hDll, GetAlgorithmFunc };

	MainLogger.logFile << "Load dll successfully for " << path << endl;
	return true;
}

IBattleshipGameAlgo* DllAlgo::GetAlgoInstance() const
{
	MainLogger.logFile << "Creating Algo instance for " << path << endl;
	return handle.second();
}

void DllAlgo::Dispose() const
{
	delete algo;
	FreeLibrary(handle.first);
}