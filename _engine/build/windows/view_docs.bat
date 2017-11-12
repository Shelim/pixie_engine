@echo off
rem *******************************************************************
rem * This simple script launches documentation
rem * 
rem * REQUIRES:
rem *     - Prebuild documentation
rem * INPUT:
rem *     1. Language - currently either 'polish' or 'english'
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [September, 6th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Fails if documentation does not exists
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

rem * Jump '\_engine\build\windows' -> '\_engine\docs'
cd "../../docs/"

rem * Set given language
set language=%~1

rem * Or to default language if omitted
if "%~1"=="" set language=English

rem * Should we fail here...
if exist engine_%language%.chm goto ok1 

	rem * Output some info
	echo Documentation does not exists
	
	rem * Fail execution
	goto failed
	
rem * Ok, we did not fail!
:ok1

rem * Open documentation
start engine_%language%.chm

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