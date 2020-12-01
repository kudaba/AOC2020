#include "TMP_Precompiled.h"
#include "GE_ToolsInitialize.h"

// Todo: Input additional modules
// #include "TMP_ToolsModule.h"

// Todo: If/When you remove GameExample, make sure to add in the dependencies it was bringing in

//-------------------------------------------------------------------------------------------------
// Initialize any dependent projects and create any project local modules here. Ideally any other
// initialization should reside in the modules themselves
//-------------------------------------------------------------------------------------------------
void TMP_InitializeTools()
{
	GE_InitializeTools();

	// Todo: Register additional modules
	// TMP_ToolsModule::Create();
}
//-------------------------------------------------------------------------------------------------
// Destroy modules then shutdown dependent projects
//-------------------------------------------------------------------------------------------------
void TMP_ShutdownTools()
{
	// Todo: Cleanup additional modules
	// TMP_ToolsModule::Destroy();

	GE_ShutdownTools();
}
