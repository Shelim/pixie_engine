@echo off
rem *******************************************************************
rem * This script is called by build_XXX as process of the build.
rem *     It routes its argument to Windows ANT script
rem * 
rem * REQUIRES:
rem *     - Depedencies
rem *     - Ant build system
rem *     - Path setup
rem * INPUT:
rem *     1. Valid Unix name of existing project
rem *     2. Either 'iterative' or 'full'
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
rem *     - It run full or iterative build of the project
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

setlocal enabledelayedexpansion

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
if "%~2"=="iterative" goto ok2
if "%~2"=="full" goto ok2
	
	rem * ... display error
	echo Wrong argument given! Expected one of 'iterative', 'full', given: '%~2'

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

if "%~2"=="iterative" goto build_iterative
if "%~2"=="full" goto build_full

:build_iterative

rem * Execute ant file
call ant -buildfile "_engine\build\windows\ant.xml" prebuild_iterative -Dproject_to_build="%~1" %arg1% %arg2% %arg3% %arg4% %arg5% %arg6% %arg7%

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok3 

	rem * Output some info
	echo Ant build failed!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok3

rem * Jump '' -> '\_engine\build\windows'
cd _engine\build\windows

set PIXIE_PROJECT_TO_BUILD=%~1
set PIXIE_ADDITIONAL_INCLUDES=
set PIXIE_ADDITIONAL_LIBRARIES=

for /d %%a in ("..\..\dependency\library\*") do (
	set "PIXIE_ADDITIONAL_INCLUDES=!PIXIE_ADDITIONAL_INCLUDES! -I..\..\_engine\dependency\library\%%~nxa\include"
	set "PIXIE_ADDITIONAL_LIBRARIES=!PIXIE_ADDITIONAL_LIBRARIES! /LIBPATH:..\..\_engine\dependency\library\%%~nxa\lib\windows_x86"
	
	for /r %%l in ("..\..\dependency\library\%%~nxa\lib\windows_x86\*") do (
		echo %%~nxl
	)
)

setlocal DISABLEDELAYEDEXPANSION

rem * Jump '\_engine\build\windows' -> '\PROJECT_TO_BUILD\!build'
cd "..\..\..\%~1\!build"

copy "..\..\_engine\build\windows\fbuild.bff" ".\fbuild.bff"

fbuild.exe -cache -fastcancel -summary -report

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok4 

	rem * Output some info
	echo FBuild build failed!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok4

rem * Jump '\PROJECT_TO_BUILD\!build' -> '\'
cd "..\.."

rem * Execute ant file
call ant -buildfile "_engine\build\windows\ant.xml" postbuild_iterative -Dproject_to_build="%~1" %arg1% %arg2% %arg3% %arg4% %arg5% %arg6% %arg7%

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok5 

	rem * Output some info
	echo Ant build failed!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok5

rem * Finalize successfully
goto ok

:build_full
rem * Execute ant file
call ant -buildfile "_engine\build\windows\ant.xml" build_full -Dproject_to_build="%~1" %arg1% %arg2% %arg3% %arg4% %arg5% %arg6% %arg7%

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok6 

	rem * Output some info
	echo Ant build failed!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok6

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