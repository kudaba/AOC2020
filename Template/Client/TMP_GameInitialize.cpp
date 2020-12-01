#include "TMP_Precompiled.h"
#include "GE_GameInitialize.h"

// Todo: Input additional modules
// #include "TMP_GameModule.h"

// Todo: If/When you remove GameExample, make sure to add in the dependencies it was bringing in

//-------------------------------------------------------------------------------------------------
// Initialize any dependent projects and create any project local modules here. Ideally any other
// initialization should reside in the modules themselves
//-------------------------------------------------------------------------------------------------
void TMP_InitializeGame()
{
	GE_InitializeGame();

	// Todo: Register additional modules
	// TMP_GameModule::Create();
}
//-------------------------------------------------------------------------------------------------
// Destroy modules then shutdown dependent projects
//-------------------------------------------------------------------------------------------------
void TMP_ShutdownGame()
{
	// Todo: Cleanup additional modules
	// TMP_GameModule::Destroy();

	GE_ShutdownGame();
}
