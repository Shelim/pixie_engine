@echo off
rem *******************************************************************
rem * This will route new project command to internals
rem * 
rem * REQUIRES:
rem *     - See '\_engine\build\windows\generate_new_project.bat'
rem * INPUT:
rem *     (Interactive)
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [October, 6th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Asks for data, then route it to batch script
rem *     - Reask for data on wrong input
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

rem * Set console color
color 07

rem * Mark current directory
pushd %~dp0

rem * Jump '\_command\windows' -> '\'
cd ..\..

rem * Verify that we have downloaded all depedencies
call _engine\build\windows\has_depedencies.bat

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok1 

	rem * Output some info
	echo Please, use script 'download_dependencies.bat' first!

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok1

rem * Set default values for project
set unix_project_name=
set full_project_name=

rem * Jump '\' -> '\_engine\build\windows'
cd _engine\build\windows

rem * Ask user for Unix Project Name
:unix_project_name_set
echo Enter unix name of your project (lowercase, no spaces nor special characters)
set /p "unix_project_name=Enter: " || goto :unix_project_name_set

rem * Validate user input
call validate_unix_project_name.bat "%unix_project_name%"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2 

	rem * Output some info
    echo Incorrect unix project name! Try again...
	
	rem * ... and ask for name again
    goto unix_project_name_set

rem * Ok, we did not fail!
:ok2

rem * Ask user for Full Project Name
:full_project_name_set
echo Enter full name of your project (should be a valid filename!) 
set /p "full_project_name=Enter: " || goto :full_project_name_set

rem * Verify Full Project Name
call validate_full_project_name.bat "%full_project_name%"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok3

	rem * Output some info
    echo Incorrect full name of project! Try again...
	
	rem * ... and ask for name again
    goto full_project_name_set

rem * Ok, we did not fail!
:ok3

rem * Do an actual build of the new project
call build_new_project.bat "%unix_project_name%" "%full_project_name%"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok4

	rem * Output some info
	echo Failed to create new project
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok4

rem * Finalize successfully
goto ok

rem * In case of failure
:failed

rem * Set color to red
color CF

rem * Output some info
echo Failed!

rem * Keep console Window open
pause

rem * End script
endlocal
	
rem * Get back current directory
popd

rem * Kill batch script
exit 1

rem * Ok, we did not fail!
:ok

rem * Set color to green
color 2F

rem * Output some info
echo All Done!

rem * Keep console Window open
pause

rem * End script
endlocal

rem * Get back current directory
popd

rem * Return OK
exit 0