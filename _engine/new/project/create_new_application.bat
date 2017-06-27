@echo off
pushd %~dp0
call "./../_engine/new/application.bat" "$EMPTY_UNIX_NAME$"
popd