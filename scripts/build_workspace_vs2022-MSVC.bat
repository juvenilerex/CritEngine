@echo off
echo Generating Visual Studio 2022 Project Files...
pushd %~dp0\..\
call thirdparty\bin\premake\premake5.exe vs2022 --toolchain=msc
popd
pause