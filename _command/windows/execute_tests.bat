@echo off
rem *******************************************************************
rem * This will create new test project, then run tests, then generate a raport
rem * 
rem * REQUIRES:
rem *     - Working build system
rem *     - Windows platform
rem *     - Dependencies downloaded
rem * INPUT:
rem *     (None)
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [February, 26th 2018]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Builds project pixie_tester (it should not have been created manuall!)
rem *     - Do not fail on failed test (instead it builds test result)
rem *******************************************************************
rem * Copyright (c) 2018 Piotr Kosek
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

rem * Jump '\' -> '\_engine\build\windows'
cd _engine\build\windows

rem * Do an actual build of the new project
call build_new_project.bat "pixie_tester" "The Pixie Tester App"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2

	rem * Output some info
	echo Failed to create new project
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok2


rem * Execute actual script (build)
call builder_actual_windows.bat pixie_tester full "build_is_iterative="false"" "build_typename="tester"" "build_is_debug="true"" "build_is_portable="true"" "build_is_deploy="false"" "build_is_final="false"" "build_is_tester="true""

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok3 

	rem * Output some info
	echo Failed to build tester build

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok3

rem * Jump '\_engine\build\windows' -> '\'
cd ..\..\..

set CODE_ROOT=%cd%

rem * Jump '\' -> '\pixie_tester\!build\windows_x64_tester\bin'
cd pixie_tester\!build\windows_x64_tester\bin

echo Executing test...

rem * Register all standard paths form build system
call ..\..\..\..\_engine\build\windows\setup_path.bat

rem * Execute application
call opencppcoverage --export_type cobertura:cov.xml --export_type html:coverage_raport --module "pixie_tester" --source "%CODE_ROOT%\_engine\code\*" --source "%CODE_ROOT%\pixie_tester\code\*" --optimized_build -- "pixie_tester.exe" --gtest_output=xml

rem * Jump '\pixie_tester\!build\windows_x64_tester\bin' -> '\'
cd ..\..\..\..

rem * Remove directory with old results
rmdir /s /q "!test_results"

rem * ... and remake it
mkdir "!test_results"

rem * Make directory for coverage
mkdir "_engine/!coverage"

rem * Move code coverage stats
move "pixie_tester\!build\windows_x64_tester\bin\cov.xml" "_engine\!coverage\cov.xml"

rem * Move code coverage raport
move "pixie_tester\!build\windows_x64_tester\bin\coverage_raport" "!test_results\coverage_raport"

rem * Move actual results of test
move "pixie_tester\!build\windows_x64_tester\bin\test_detail.xml" "!test_results\test_detail.xml"

rem * Jump '\' -> '\!test_results'
cd !test_results

msxsl test_detail.xml "..\_engine\build\common\test.xslt" > "raport.html"

rem * Jump '\!test_results' -> '\'
cd ..

rem * Remove project; We do not need it now
rmdir /s /q "pixie_tester"

rem * Open code coverate in browser window
call "!test_results\coverage_raport\index.html"

rem * Open code coverate in browser window
call "!test_results\raport.html"

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