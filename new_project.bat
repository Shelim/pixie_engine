@echo off
pushd %~dp0

IF NOT EXIST "./_engine/depedency" (
    echo Use download_dependencies.bat to download required depedencies before doing anything else!
	pause
	exit /b 1
)

call "./_engine/new/project.bat"
popd