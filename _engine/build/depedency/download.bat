@echo off
setlocal
cd ../..
:PROMPT
SET /P AREYOUSURE=This will wipe out existing depedencies and re-download them all. IT WILL TAKE SOME TIME, maybe even a few hours! Are you sure to proceed? y/(n)
IF /I "%AREYOUSURE%" NEQ "Y" GOTO END

echo Deleting existing depedencies...

IF EXIST "depedency" (
    rmdir "depedency" /s /q
)

mkdir "depedency"

echo Done!

echo Downloading depedencies (warning: THIS WILL TAKE SOME TIME!)...

IF NOT EXIST "_build/depedency" (
    mkdir "_build/depedency"
)

powershell -Command "(New-Object Net.WebClient).DownloadFile('http://open-source.kosek.com/pixie_engine/depedencies.zip', '_build/depedency/depedencies.zip')"

echo Done!

echo Unpacking depedencies (warning: THIS WILL TAKE !EVEN MORE! TIME!)...
echo ... like several minutes (or even few hours). Please, wait patiently...
echo ... you have been warned.

call build/depedency/zipjs.bat unzip -source %CD%\_build\depedency\depedencies.zip -destination %CD%\depedency

echo Done!
echo Deleting depedencies zip file...

rmdir "_build/depedency" /s /q

echo Done!

echo All Done!

pause

:END

endlocal