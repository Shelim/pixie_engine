@echo off
rem *******************************************************************
rem * Creates new project for Pixie
rem * 
rem * REQUIRES:
rem *     - See '\_engine\build\new_project\ant.xml' (build_new_project target)
rem * INPUT:
rem *     1. Valid Unix name of the new project
rem *     2. Valid full project name
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [October, 6th 2017]:
rem *         - Initial version
rem *************************************************************************************************
rem * CONSTRAINS:
rem *     - The script will error if any of the given names are invalid
rem *******************************************************************
rem * Copyright (c) 2017 Piotr Kosek
rem * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
rem * and associated documentation files (the "Software"), to deal in the Software without restriction,
rem * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
rem * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
rem * subject to the following conditions:
rem *
rem * The above copyright notice and this permission notice shall be included in all copies
rem * or substantial portions of the Software.
rem *
rem * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
rem * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
rem * A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
rem * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
rem * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
rem * DEALINGS IN THE SOFTWARE.
rem *******************************************************************

rem * Mark current directory
pushd %~dp0

rem * Output some info
echo Generating new project...

rem * Treat all variables as local
setlocal

rem * Verify arguments

rem * Use more meaningful variable names
set unix_project_name=%~1
set full_project_name=%~2

rem * Validate user input
call validate_unix_project_name.bat "%unix_project_name%"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok1
	
	rem * ... display error
    echo Incorrect unix project name!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok1

rem * Verify Full Project Name
call validate_full_project_name.bat "%full_project_name%"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2
	
	rem * ... display error
    echo Incorrect full name of project! Try again...
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok2

rem * Jump '\_engine\build\windows' -> '\_engine'
cd ..\..\

rem * Register all standard paths form build system
call build\windows\setup_path.bat

rem * Execute ant file
call ant -v -buildfile "build\new_project\ant.xml" new_project -Dunix_project_name="%unix_project_name%" -Dfull_project_name="%full_project_name%"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok3

	rem * Output some info
	echo Ant build failed!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok3

rem * Finalize successfully
goto ok

rem * In case of failure
:failed
	
rem * Get back current directory
popd

rem * Kill batch script
exit /B 1

rem * Ok, we did not fail!
:ok

rem * Get back current directory
popd

rem * Return OK
exit /B 0