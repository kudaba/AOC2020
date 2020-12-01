#include "GS_Include.h"
#include "GS_App.h"
#include "GS_Globals.h"
#include "GS_ModuleManager.h"
#include "GC_Initialize.h"

#include "TMP_GameInitialize.h"

#if IS_EDITOR
#include "TMP_ToolsInitialize.h"
#endif

char const* theAppName = "Template";

//-------------------------------------------------------------------------------------------------
// Common app startup code
//-------------------------------------------------------------------------------------------------
int RunApp(GS_App& app)
{
	// Startup
	TMP_InitializeGame();

#if IS_EDITOR
	TMP_InitializeTools();
	GC_VERIFY(GS_ModuleManager::Instance().Load("StartScreenModule"));
	GC_VERIFY(GS_ModuleManager::Instance().Load("ToolsModule"));
#else
	GC_VERIFY(GS_ModuleManager::Instance().Load("GameModule"));
#endif

	// Main Loop
	app.Run();

	// Shutdown
	GS_ModuleManager::Instance().UnloadAll();

#if IS_EDITOR
	TMP_ShutdownTools();
#endif

	TMP_ShutdownGame();

	app.Shutdown();

	return app.GetExitCode();
}

//-------------------------------------------------------------------------------------------------
// Platform specific startup code
//-------------------------------------------------------------------------------------------------
#if IS_WINDOWS
#include <Platform/Windows/Windows.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	// Default location for current/data/bin directory is AppName/Bin
	GC_FilePath branchToCurrent = GC_Path::Combine(theAppName, "Bin");
	GC_InitializeSettings settings(theAppName);
	settings.BranchToCurrentDirectory = branchToCurrent;
	settings.CommandLine = lpCmdLine;

	GS_App app;
	app.Startup(settings);

	GS_Globals::Instance().Set("hInstance", hInstance);
	GS_Globals::Instance().Set("hPrevInstance", hPrevInstance);
	GS_Globals::Instance().Set("nCmdShow", nCmdShow);

	return RunApp(app);
}
#else // IS_WINDOWS // All other platforms

int main(int argc, char* argv[])
{
	GC_InitializeSettings settings(theAppName);
	settings.ArgV = argv;
	settings.ArgC = argc;

	GS_App app;
	app.Startup(settings);

	return RunApp(app);
}

#endif // IS_WINDOWS
