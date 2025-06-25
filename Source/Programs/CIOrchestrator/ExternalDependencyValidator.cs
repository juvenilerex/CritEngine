using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CIOrchestrator
{
    public class ExternalDependencyValidator
    {
        public static bool Exists(string DependencyName)
        {
            Process testProcess = new Process();
            testProcess.StartInfo.FileName = "where";
            testProcess.StartInfo.ArgumentList.Add(DependencyName); 
            testProcess.StartInfo.RedirectStandardOutput = true;
            testProcess.StartInfo.RedirectStandardError = true;

            testProcess.Start();
            testProcess.WaitForExit();

            return testProcess.ExitCode == 0;
        }
    }
}
