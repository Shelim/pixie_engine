@echo off
rem *******************************************************************
rem * This will download depedencies for project from official Kosek.com
rem *     servers. It is provided as convenient function that speeds up
rem *     environment setup. You should call this only once
rem * 
rem * REQUIRES:
rem *     - Internet connection!
rem *     - Depedencies build on server (use internal_pack_and_upload_dependencies.bat
rem *           to refresh server build -> you will need a password for FTP!)
rem *     - Zip decompressor (by default here: '\_engine\build\windows\zipjs.bat'
rem * INPUT:
rem *     (None)
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [September, 8th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - Does NOT requires depedencies to be downloaded apriori (obvious..?)
rem *     - Should run in fresh environment (ie. does not have external depedencies)
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

rem * Jump '\_engine\build\windows' -> '\_engine'
cd ../..

rem * Ask for confirmation (we will download gigabytes of data!)
:PROMPT
set /P AREYOUSURE=This will wipe out existing depedencies and re-download them all. IT WILL TAKE SOME TIME, maybe even a few hours! Are you sure to proceed? y/N: 

rem * Should we fail here...
if /I "%AREYOUSURE%" == "Y" goto ok1

	rem * ... display error
    echo Cancelled by user!
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok1


rem * Output some info
echo Deleting existing depedencies...

rem * If dependency directory existed, wipe it out...
if exist "dependency" rmdir "dependency" /s /q

rem * ... and create anew (should be empty folder after this point)
mkdir "dependency"

rem * Output some info
echo Done!

rem * Output some info
echo Downloading depedencies (warning: THIS WILL TAKE SOME TIME!)...

rem * Make sure the !build directory exists
if not exist !build mkdir !build

rem * Make sure the !build\dependency directory exists
if not exist "!build\dependency" mkdir "!build\dependency"

rem * I actually don't like powershell here (used to download depedencies)... Maybe we could use something else..?
powershell -Command "(New-Object Net.WebClient).DownloadFile('http://open-source.kosek.com/pixie_engine/depedencies.zip', '!build/dependency/depedencies.zip')"

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok2

	rem * Output some info
	echo Failed to download files from FTP (do you have working network connection?)
	
	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok2

rem * Output some info
echo Done!

rem * Output some info
echo Unpacking depedencies (warning: THIS WILL TAKE !EVEN MORE! TIME!)...
echo ... like several minutes (or even few hours). Please, wait patiently...
echo ... you have been warned.

rem * Unpack zip archive!
call build\windows\zipjs.bat unzip -source %CD%\!build\dependency\depedencies.zip -destination %CD%\dependency

rem * Should we fail here...
if %ERRORLEVEL% == 0 goto ok3 

	rem * Output some info
	echo Failed to unzip downloads. Use your favorite zip program to unpack '%CD%\!build\dependency\depedencies.zip' into '%CD%\dependency'!

	rem * Fail execution
	goto failed

rem * Ok, we did not fail!
:ok3

rem * Output some info
echo Done!
echo Deleting depedencies zip file...

rem * Remove temporary directory
rmdir "!build/dependency" /s /q

rem * Output some info
echo Done!

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