using System;

namespace CIOrchestrator
{
    public class ProcessEnvInjector
    {
        // The purpose of this class is to inject some common paths for our dependencies into this process' environment path variable.
        static string[] commonMSBuildSystemPaths = [
            "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe"
            ];

        public static void Inject()
        {
            
            if(!ExternalDependencyValidator.Exists("MSBuild.exe"))
            {
                
                foreach (string path in commonMSBuildSystemPaths)
                {
                    if (File.Exists(path))
                    {
                        Console.WriteLine("Found MSBuild.exe installation!");
                        Environment.SetEnvironmentVariable("PATH", Path.GetDirectoryName(path), EnvironmentVariableTarget.Process);
                        break;
                    }
                }
            }
            
        }
    }
}
