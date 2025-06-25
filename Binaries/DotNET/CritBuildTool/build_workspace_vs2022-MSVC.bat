@echo off
echo Generating Visual Studio 2022 Project Files...
call bin\Debug\net8.0\CritBuildTool.exe vs2022 msc-v143
popd
pause