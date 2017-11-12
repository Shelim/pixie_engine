@echo off
rem *******************************************************************
rem * This will validate first argument as Unix Project Name and set 
rem *     errorlevel to 1 in case of any problems
rem * 
rem * REQUIRES:
rem *     - Name to check passed as first argument
rem * INPUT:
rem *     1. Name to validate
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [October, 6th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Unix Project Name should be a consts of lowercase letters and numbers and underscores
rem *     - It should NOT begin with number through
rem *     - It should NOT begin with underscore (it is reserved engine name!)
rem *     - The reason: the Unix Project Name can be used as variable name in C++
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

rem * Verify that argument meets the criteria from constrains
echo %~1|findstr /r "^[abcdefghijklmnopqrstuvwxyz][abcdefghijklmnopqrstuvwxyz_0123456789]*$">nul 2>&1

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok1

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok1

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