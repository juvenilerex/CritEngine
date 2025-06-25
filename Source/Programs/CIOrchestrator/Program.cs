
using System.Diagnostics;
using System.Linq;
using System.Reflection;

namespace CIOrchestrator
{
    class Program
    {

        static int Main(string[] args)
        {
            // This tool handles CI orchestration through automating the building of the main engine and invoking tests related the engine.
            // First we need to build the engine.
            // Then we need to run any related tests for the engine.
            // Repeat for all configurations and supported compilers.
            // Finally we report the final numbers to the console.

            // TODO: For now this is hardcoded, in the future we need a way to grab these numbers from a reliable source.
            string WorkspaceFormat = "vs2022";
            string[] SupportedToolsets = ["msc-v143", "clang"];
            string[] WorkspaceConfigurations = ["Debug", "ReleaseDev", "Release"];

            List<bool> BuildToolResults = new List<bool> { };
            List<bool> CompileResults = new List<bool> { };

            ProcessEnvInjector.Inject();

            foreach (string CurrentToolset in SupportedToolsets)
            {
                // First we build the engine workspace using the selected Workspace Format.
                int BuildToolResult = RunBuildTool(WorkspaceFormat, CurrentToolset);
                BuildToolResults.Add(BuildToolResult == 0);

                foreach (string WorkspaceConfig in WorkspaceConfigurations)
                {
                    // Then we attempt to compile the engine with the selected config.
                    int CompileResult = CompileWorkspace(WorkspaceFormat, WorkspaceConfig);
                    CompileResults.Add(CompileResult == 0);

                    // Finally we can run the compiled binary with it's test flag on to see if testing runs correctly
                }
            }

            for (int i = 0; i < SupportedToolsets.Length; i++)
            {
                Console.WriteLine($"Workspace Build '{SupportedToolsets[i]}' {(BuildToolResults[i] ? "Succeeded" : "Failed")}.");
                for (int j = 0; j < WorkspaceConfigurations.Length; j++)
                {
                    Console.WriteLine($"Compile '{SupportedToolsets[i]}'/'{WorkspaceConfigurations[j]}' {(CompileResults[i] ? "Succeeded" : "Failed")}.");
                }
            }

            return 0;
        }

        static int RunBuildTool(string workspaceFormat, string toolset)
        {

            //TODO: Put these paths in some config file.
            string RootPath = Path.GetFullPath(Path.Combine(Assembly.GetExecutingAssembly().Location, "../../../.."));
            string BuildToolPath = Path.GetFullPath(Path.Combine(RootPath, "../CritBuildTool/bin/Debug/net8.0/CritBuildTool.exe"));

            Console.WriteLine($"Running CritBuildTool with workspace='{workspaceFormat}' and toolset='{toolset}'.");

            Process buildToolProcess = new Process();
            buildToolProcess.StartInfo.FileName = BuildToolPath;
            buildToolProcess.StartInfo.ArgumentList.Add(workspaceFormat);
            buildToolProcess.StartInfo.ArgumentList.Add(toolset);
            buildToolProcess.StartInfo.RedirectStandardOutput = true;
            buildToolProcess.StartInfo.RedirectStandardError = true;
            //buildToolProcess.StartInfo.CreateNoWindow = true;

            buildToolProcess.OutputDataReceived += new DataReceivedEventHandler(OutputHandler);
            buildToolProcess.ErrorDataReceived += new DataReceivedEventHandler(OutputHandler);
            buildToolProcess.Start();
            buildToolProcess.BeginOutputReadLine();
            buildToolProcess.BeginErrorReadLine();

            buildToolProcess.WaitForExit();
            string exitMessage = buildToolProcess.ExitCode == 0 ? "CritBuildTool Ran Successfully." : $"CritBuildTool Failed with code {buildToolProcess.ExitCode}.";
            Console.WriteLine(exitMessage);

            return buildToolProcess.ExitCode;

            static void OutputHandler(object sendingProcess, DataReceivedEventArgs args)
            {
                Console.WriteLine(args.Data);
            }
        }

        static int CompileWorkspace(string workspaceFormat, string workspaceConfigutation)
        {
            if (workspaceFormat != "vs2022")
            {
                Console.WriteLine("Compilation only works with vs2022 currently.");
                return 1;
            }

            //TODO: Put these paths in some config file.
            string RootPath = Path.GetFullPath(Path.Combine(Assembly.GetExecutingAssembly().Location, "../../../.."));
            string WorkspacePath = Path.GetFullPath(Path.Combine(RootPath, "../../../CritEngine.sln"));
            string WorkspaceCompilerExecutable = "MSBuild.exe";

            Console.WriteLine($"Running MSBuild.exe on '{workspaceFormat}'.");


            Process compilerProcess = new Process();
            compilerProcess.StartInfo.FileName = WorkspaceCompilerExecutable;
            compilerProcess.StartInfo.ArgumentList.Add(WorkspacePath);
            compilerProcess.StartInfo.ArgumentList.Add($"/p:Configuration={workspaceConfigutation}");
            compilerProcess.StartInfo.RedirectStandardOutput = true;
            compilerProcess.StartInfo.RedirectStandardError = true;
            //compilerProcess.StartInfo.CreateNoWindow = true;

            compilerProcess.OutputDataReceived += new DataReceivedEventHandler(OutputHandler);
            compilerProcess.ErrorDataReceived += new DataReceivedEventHandler(OutputHandler);
            compilerProcess.Start();
            compilerProcess.BeginOutputReadLine();
            compilerProcess.BeginErrorReadLine();

            compilerProcess.WaitForExit();
            string exitMessage = compilerProcess.ExitCode == 0 ? "MSBuild.exe Ran Successfully." : $"MSBuild.exe Failed with code {compilerProcess.ExitCode}.";
            Console.WriteLine(exitMessage);

            return compilerProcess.ExitCode;

            static void OutputHandler(object sendingProcess, DataReceivedEventArgs args)
            {
                Console.WriteLine(args.Data);
            }
        }
    }
}