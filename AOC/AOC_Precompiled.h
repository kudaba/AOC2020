#pragma once

#define BASE_FIXTURE AOC_TestFixture

#include "GC_Include.h"
#include "GC_Mutex.h"
#include "GC_UnitTest.h"
#include "GC_DynamicArray2D.h"

#define RUN_TESTS IS_DEBUG

class AOC_TestFixture : public GC_TestFixture
{
protected:
	void OnSetup() override { myTimer.Reset(); }
	void OnTearDown() override
	{
		Printf(GC_Strfmt<256>("{}/{} Completed in %5dms.\n", TestGroup(), TestName(), myTimer.GetElapsedMillis()));
	}
private:
	GC_HighResTimer myTimer;
};
