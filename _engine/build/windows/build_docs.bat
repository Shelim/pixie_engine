@echo off
rem *******************************************************************
rem * Builds documentation for Pixie
rem * 
rem * REQUIRES:
rem *     - Doxygen
rem *     - Commented code
rem * INPUT:
rem *     1. Language - currently one of 'polish', 'english'
rem *     2. Format - currently either 'chm' and 'pdf'
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [September, 6th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Does not write outside subdirectory of docs (results) and !build (temporaries)
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

rem * Set default variables
set generate_pdf=NO
set generate_chm=NO

rem * Set format and reset to default if needed
set format=%~2
if NOT "%~2"=="chm" if NOT "%~2"=="pdf" set format=chm

rem * Set given format for generation
if "%format%"=="chm" set generate_chm=YES
if "%format%"=="pdf" set generate_pdf=YES

rem * Make sure the !build directory exists
IF NOT EXIST !build mkdir !build

rem * Jump '\_engine' -> '\_engine\!build'
cd !build

rem * If doxygen directory existed, wipe it out...
IF EXIST doxygen_%language% rmdir "doxygen_%language%" /s /q

rem * ... and create anew (should be empty folder after this point)
mkdir doxygen_%language%

rem * Jump '\_engine\!build' -> '\_engine\!build\doxygen_%language%'
cd doxygen_%language%

rem * If PDF make latex output
if "%format%"=="pdf" mkdir latex

rem * If PDF copy manifest here
if "%format%"=="pdf" xcopy /s "..\..\manifest\doxygen" "latex\"

rem * Jump '\_engine\!build\doxygen_%language%' -> '\_engine'
cd ..\..

rem * If docs directory does not exists, create it
IF NOT EXIST "!docs" mkdir "!docs"

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

rem * Jump '\_engine\build\windows' -> '\_engine\!build\doxygen_%language%\latex'
cd ..\..\!build\doxygen_%language%\latex

rem * Replace links in pdf
if "%format%"=="pdf" for /r %%v in (*.tex) do sed -i -E "s/doxyref\{[^}]+\}\{str\.\}(\{[^}]+\})/ref\1/gi" "%%v"
rem if "%format%"=="pdf" for /r %%v in (*.tex) do sed -i -E "s/([a-zA-Z0-9.\-]).\+([A-Z0-9])/\1\2/g" "%%v"

rem * Jump '\_engine\!build\doxygen_%language%\latex' -> '\_engine'
cd ..\..\..\

rem * Build actual PDF
if "%format%"=="pdf" call !build\doxygen_%language%\latex\make.bat

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2 

	rem * Output some info
	echo Failed to generate pdf
	
	rem * Fail execution
	goto failed
	
rem * Ok, we did not fail!
:ok2

if "%format%"=="pdf" copy /Y !build\doxygen_%language%\latex\refman.pdf !docs\book_%language%.pdf

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