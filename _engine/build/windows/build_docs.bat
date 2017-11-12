@echo off
rem *******************************************************************
rem * Builds documentation for Pixie
rem * 
rem * REQUIRES:
rem *     - Doxygen
rem *     - Commented code
rem * INPUT:
rem *     1. Language - currently one of 'Polish', 'English'
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [September, 6th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Does not write outside subdirectory of docs (results) and _build (temporaries)
rem *     - Wipe out both results and temporaries directories (subdirectory of results and temporaries)
rem *     - Uses Doxygen
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

rem * Jump '\_engine\build\windows' -> '\_engine'
cd ..\..

rem * Set given language
set language=%~1

rem * Or to default language if omitted
if "%~1"=="" set language=english

rem * Make sure the _build directory exists
IF NOT EXIST _build mkdir _build

rem * Jump '\_engine' -> '\_engine\_build'
cd _build

rem * If doxygen directory existed, wipe it out...
IF EXIST doxygen_%language% rmdir "doxygen_%language%" /s /q

rem * ... and create anew (should be empty folder after this point)
mkdir doxygen_%language%

rem * Jump '\_engine\_build' -> '\_engine'
cd ..

rem * If docs directory does not exists, create it
IF NOT EXIST docs mkdir docs

rem * Register all standard paths form build system
call build\windows\setup_path.bat

rem * Jump '\_engine' -> '\_engine\build\windows'
cd build\windows

rem * Set variable to full path to here
set current_directory=%CD%

rem * Generate docs
call doxygen.exe doxyfile

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok1 

	rem * Output some info
	echo Failed to generate docs
	
	rem * Fail execution
	goto failed
	
rem * Ok, we did not fail!
:ok1

rem * Jump '\_engine\build\windows' -> '\_engine'
cd ..\..

rem * Build actual PDF
call _build\doxygen_%language%\latex\make.bat

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2 

	rem * Output some info
	echo Failed to generate pdf
	
	rem * Fail execution
	goto failed
	
rem * Ok, we did not fail!
:ok2

copy /Y _build\doxygen_%language%\latex\refman.pdf docs\pdf_%language%.pdf

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