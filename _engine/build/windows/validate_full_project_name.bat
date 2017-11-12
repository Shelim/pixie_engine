@echo off
rem *******************************************************************
rem * This will validate first argument as Full Project Name and set 
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
rem *     - Full Project Name should be a valid file name ON ALL TARGET PLATFORMS (!)
rem *     - For Windows, see this: https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247%28v=vs.85%29.aspx
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

rem * From here, we will be enabling/disabling delayed expansion 
rem * to avoid problems with special characters

setlocal enabledelayedexpansion

set name_to_validate=%~1

rem * Ensure we do not have restricted characters in file name trying to use them as 
rem * delimiters and requesting the second token in the line
for /f tokens^=2^ delims^=^<^>^:^"^/^\^|^?^*^ eol^= %%y in ("[!name_to_validate!]") do (
	rem * If we are here there is a second token, so, there is a special character
	echo Error: Non allowed characters in file name
	endlocal & goto failed
)

rem * Check MAX_PATH (260) limitation
set "my_temp_file=!cd!\!name_to_validate!" & if not "!my_temp_file:~260!"=="" (
	echo Error: file name too long
	endlocal & goto failed
)

rem * Check path inclusion, file name correction
for /f delims^=^ eol^= %%a in ("!name_to_validate!") do (
	rem * Cancel delayed expansion to avoid ! removal during expansion
	endlocal

	rem * Until checked, we don't have a valid file
	set "name_to_validate="

	rem * Check we don't have a path 
	if /i not "%%~a"=="%%~nxa" (
		echo Error: Paths are not allowed
		goto failed
	)

	rem * Check it is not a folder 
	if exist "%%~nxa\" (
		echo Error: Folder with same name present 
		goto failed
	)

	rem * ASCII 0-31 check. Check file name can be created
	2>nul ( >>"%%~nxa" type nul ) || (
		echo Error: File name is not valid for this file system
		goto failed
	)

	rem * Ensure it was not a special file name by trying to delete the newly created file
	2>nul ( del /q "%%~nxa" ) || (
		echo Error: Reserved file name used
		goto failed
	)

	rem * Everything was OK - We have a file name 
	goto ok
)

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