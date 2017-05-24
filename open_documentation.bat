@echo off
pushd %~dp0

IF NOT EXIST "./_engine/depedency" (
    echo Use download_dependencies.bat to download required depedencies before doing anything else!
	pause
	exit /b 1
)

cd _engine/build/doxygen

if exist ../../docs/engine.chm (
	call view_docs.bat
) else (
	echo Documentation does not exists, regeneraing now... Please allow a few seconds to complete
    call generate_docs.bat
	call view_docs.bat
)

popd