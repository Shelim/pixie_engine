@echo off
pushd %~dp0
call "./../_engine/build/visual_studio_2017/execute.bat" "$EMPTY_UNIX_NAME$"
popd