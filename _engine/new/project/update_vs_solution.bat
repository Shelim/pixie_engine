@echo off
pushd %~dp0
call "./../_engine/build/windows/project/execute.bat" "$EMPTY_UNIX_NAME$"
popd