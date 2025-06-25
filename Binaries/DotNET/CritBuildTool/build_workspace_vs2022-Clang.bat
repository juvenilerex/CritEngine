@echo off
echo Generating Visual Studio 2022 Project Files...
pushd %~dp0\..\
call .\Source\Programs\CritBuildTool\bin\Debug\net8.0\CritBuildTool.exe vs2022 clang
popd
pause