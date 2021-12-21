#include "AOC_Precompiled.h"
#include "GC_Initialize.h"

uint64 AOC_TestFixture::theHighResTime;

int main(int argc, char* argv[])
{
	// Initialize GC once to commit CWD
	GC_InitializeSettings settings("UnitTests");
	settings.LogFile = nullptr; // disable logging since it's costly and gets overwritten on each test
	settings.TaskPoolsEnabled = false; // unit tests don't get the thread pool, they need to control their threads manually
	settings.BranchToCurrentDirectory = "AOC";
	GC_Initialize(settings);
	GC_Shutdown();

	int ret = GC_TestFixture::RunAll(argc, argv);

	auto output = GC_StringPrinter::TimeFromHighRes(AOC_TestFixture::theHighResTime);
	printf("Tests Completed in %s.\n", output.ToString());

	return ret;
}
