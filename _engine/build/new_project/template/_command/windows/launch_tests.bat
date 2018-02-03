@echo off
rem *******************************************************************
rem * This will build tester configuration of this project then execute tests then generate tests raports
rem * 
rem * REQUIRES:
rem *     - See '\_engine\build\windows\builder_actual_windows.bat'
rem *
rem * INPUT:
rem *     (None)
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [February, 1st 2018]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - See '\_engine\build\windows\builder_actual_windows.bat'
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

rem * Jump '\$UNIX_PROJECT_NAME$\command\windows' -> '\_engine\build\windows'
cd ..\..\..\_engine\build\windows

rem * Execute actual script (build)
call builder_actual_windows.bat $UNIX_PROJECT_NAME$ full "build_is_iterative="false"" "build_typename="tester"" "build_is_debug="true"" "build_is_portable="true"" "build_is_deploy="false"" "build_is_final="false"" "build_is_tester="true""

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok1 

	rem * Output some info
	echo Failed to build tester build

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok1

rem * Jump '\_engine\build\windows' -> '\$UNIX_PROJECT_NAME$\!build\windows_x64_tester\bin'
cd ..\..\..\$UNIX_PROJECT_NAME$\!build\windows_x64_tester\bin

echo Executing application...

rem * Execute application
call pixie_tester.exe

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2 

	rem * Output some info
	echo Application exit with error level: %ERRORLEVEL%

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok2

echo Done!

echo Collecting profdata

rem * Register all standard paths form build system
call ..\..\..\..\_engine\build\windows\setup_path.bat

call llvm-profdata.exe merge -sparse default.profraw -o $UNIX_PROJECT_NAME$.profdata

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok3 

	rem * Output some info
	echo LLVM-ProfData failed!

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok3

rem * Jump '\$UNIX_PROJECT_NAME$\!build\windows_x64_tester\bin' -> '\$UNIX_PROJECT_NAME$\!build\windows_x64_tester'
cd ..

mkdir coverage

move "bin\$UNIX_PROJECT_NAME$.profdata" "coverage\$UNIX_PROJECT_NAME$.profdata"

rem * Jump '\$UNIX_PROJECT_NAME$\!build\windows_x64_tester' -> '\$UNIX_PROJECT_NAME$\!build\windows_x64_tester\coverage'
cd coverage

llvm-cov show ./../bin/pixie_tester.exe -instr-profile=$UNIX_PROJECT_NAME$.profdata > coverage.txt

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