@echo off
rem *******************************************************************
rem * This script is called by MSVC as process of the build.
rem *     It routes its argument to Windows ANT script
rem * 
rem * REQUIRES:
rem *     - Depedencies
rem *     - Ant build system
rem *     - Path setup
rem * INPUT:
rem *     1. Build step. One of 'prebuild_all', 'prebuild', 'postbuild', 'postbuild_all'
rem *     2. Valid Unix name of existing project
rem *     3 - 9. Additional arguments to be passed to ANT script as defines
rem *         (see ANT script for whole list of defines)
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [September, 17th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - This script will be called with arguments (in this order!):
rem *         - prebuild
rem *         - [for each project]
rem *             - build
rem *         - postbuild
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

rem * Treat all variables as local
setlocal

rem * Jump '\_engine\build\windows' -> '\'
cd ..\..\..

rem * Verify that we have downloaded all depedencies
call _engine\build\windows\has_depedencies.bat

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok1 
	
	rem * ... display error
    echo Plese, redownload depedencies!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok1

rem * Register all standard paths form build system
call _engine\build\windows\setup_path.bat

rem * Should we fail here to provide correct argument...
if "%~1"=="prebuild" goto ok2
if "%~1"=="build" goto ok2
if "%~1"=="postbuild" goto ok2
	
	rem * ... display error
	echo Wrong argument given! Expected one of 'prebuild', 'build', 'postbuild', given: '%~1'

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok2

rem * Up to 7 optional arguments can be passed from command line
set arg1=
set arg2=
set arg3=
set arg4=
set arg5=
set arg6=
set arg7=

rem * Handle optional arguments
if "%~3"=="" goto argument_1
if "%~4"=="" goto argument_2
if "%~5"=="" goto argument_3
if "%~6"=="" goto argument_4
if "%~7"=="" goto argument_5
if "%~8"=="" goto argument_6
if "%~9"=="" goto argument_7
goto argument_8

rem * morph optional arguments into ANT defines
:argument_8
set arg7=-D%~9
:argument_7
set arg6=-D%~8
:argument_6
set arg5=-D%~7
:argument_5
set arg4=-D%~6
:argument_4
set arg3=-D%~5
:argument_3
set arg2=-D%~4
:argument_2
set arg1=-D%~3
:argument_1

rem * Execute ant file
call ant -v -buildfile "_engine\build\windows\ant.xml" %~1 -Dproject_to_build="%~2" %arg1% %arg2% %arg3% %arg4% %arg5% %arg6% %arg7%

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