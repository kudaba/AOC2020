#include "AOC_Precompiled.h"
#include "GC_Initialize.h"

int main(int argc, char* argv[])
{
	// Initialize GC once to commit CWD
	GC_InitializeSettings settings("UnitTests");
	settings.LogFile = nullptr; // disable logging since it's costly and gets overwritten on each test
	settings.TaskPoolsEnabled = false; // unit tests don't get the thread pool, they need to control their threads manually
	settings.BranchToCurrentDirectory = "AOC";
	GC_Initialize(settings);
	GC_Shutdown();

	return GC_TestFixture::RunAll(argc, argv);
}
