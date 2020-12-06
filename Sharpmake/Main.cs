using Sharpmake;
using Graphium;

//-------------------------------------------------------------------------------------------------
// Pull in Graphium's Sharpmake files as a separate project
//-------------------------------------------------------------------------------------------------
[module: Sharpmake.Package(@"..\Graphium\Sharpmake\Include.cs")]

//-------------------------------------------------------------------------------------------------
// Set the name for the project generated by this Sharpmake file
//-------------------------------------------------------------------------------------------------
[module: Sharpmake.DebugProjectName("AOC")]

namespace Template
{
	//-------------------------------------------------------------------------------------------------
	// The Client 'is' the game. It's should contain everything to run the game for an End User
	//-------------------------------------------------------------------------------------------------
	[Generate]
	class AOCProject : BaseCPPTest
	{
		public AOCProject()
		{
			SharpmakeToGlobalRoot = SharpmakeToRoot = @"\..";

			TestProject = "AOC";
			ShortName = "AOC";
			SourceRootPath = @"[project.ToRoot]\[project.TestProject]";
			SourceFilesExtensions.Add("txt");
		}

		public override void ConfigureAll(Configuration conf, Target target)
		{
			base.ConfigureAll(conf, target);
			conf.AddPrivateDependency<GraphiumCoreProject>(target);
			conf.SolutionFolder = "AOC";
		}
	}
	
	//-------------------------------------------------------------------------------------------------
	// Android project that will package up everything up, deploy to a device or emulator and allow debugging
	//-------------------------------------------------------------------------------------------------
	[Generate]
	class AOCSolution : BaseSolution
	{
		public AOCSolution()
		{
			SharpmakeToGlobalRoot = SharpmakeToRoot = @"\..";
			Name = "AOC";
		}

		public override void AddProjects(Configuration conf, Target target)
		{
			GraphiumExampleGameSolution.AddDefaultProject<AOCProject>(conf, target);
			conf.AddProject<GraphiumCoreTestProject>(target);
		}
	}

	//-------------------------------------------------------------------------------------------------
	// Sharpmake's equivalent of the Main function. Add any custom setup here and any additional solutions
	// you want generated.
	//-------------------------------------------------------------------------------------------------
	public static class Entry
    {
        [Sharpmake.Main]
        public static void SharpmakeMain(Arguments arguments)
        {
			var cmdLine = new Parameters();
			CommandLine.ExecuteOnObject(cmdLine);

            KitsRootPaths.SetKitsRoot10ToHighestInstalledVersion();

			// important this happens after command line processing
			Globals.SharpmakeToGlobalRoot += @"\..";
			arguments.Generate<AOCSolution>();
       }
    }
}
