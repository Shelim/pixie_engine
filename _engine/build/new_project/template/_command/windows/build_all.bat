@echo off
rem *******************************************************************
rem * This will build all configurations of this project
rem * 
rem * REQUIRES:
rem *     - See '\_engine\build\windows\builder_actual.bat'
rem *
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [November, 2nd 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - See '\_engine\build\windows\builder_actual.bat'
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

rem * Should we fail here to provide correct argument...
if "%~1"=="" goto real

rem * Execute script
echo | call %~1

rem * Should we fail here...
if errorlevel 1 (

	rem * Output some info
	echo Failed to build this project

	rem * Keep console Window open
	pause
	
	rem * Fail execution
	call failed
	
	rem * Fail execution
	exit 1
	
)

rem * Complete execution
goto ok

rem * Go to normal execution
:real

rem * From here, we will be enabling/disabling delayed expansion 
rem * to avoid problems with special characters
setlocal enableextensions enabledelayedexpansion

rem * Search for all build scripts
for %%f in (build_full_*.bat) do (

rem * Output some info
echo Running %%~nf

rem * Execute build script
start /wait build_all.bat %%f

rem * Should we fail here...
if errorlevel 1 (

	rem * Output some info
	echo Failed to prebuild debug build

	rem * Fail execution
	call failed
	
	rem * Fail execution
	exit 1
	
)

rem * Output some info
echo Done!

)

)

)

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

rem * Keep console Window open if calling script directly
if "%~1"=="" pause

rem * End script
endlocal

rem * Get back current directory
popd

rem * Return OK
exit 0