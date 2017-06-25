@echo off
pushd %~dp0
call "./../_engine/build/new_project/new_application.bat" "$EMPTY_UNIX_NAME$"
call "./update_vs_solution.bat"
popd