rem @echo off
pushd %~dp0

set unix_filename=example_app
set full_filename="My Example Application"
set exe_filename=game

:unix_filename_set
set /p unix_filename=Enter unix filename of your project (lowercase, no spaces nor special characters):

echo %unix_filename%|findstr /r "^[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789]*$">nul 2>&1
if errorlevel 1 (
    echo Incorrect unix filename! Try again...
    goto unix_filename_set
)

:full_filename_set
rem Retrieve filename. On empty input ask again
set /p "full_filename=Enter full name of your project (should be a valid filename!): " || goto :full_filename_set
 
	rem See Naming Files, Paths, and Namespaces
    rem     https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247%28v=vs.85%29.aspx

    rem NOTE: From here, we will be enabling/disabling delayed expansion 
    rem       to avoid problems with special characters

    setlocal enabledelayedexpansion
    rem Ensure we do not have restricted characters in file name trying to use them as 
    rem delimiters and requesting the second token in the line
    for /f tokens^=2^ delims^=^<^>^:^"^/^\^|^?^*^ eol^= %%y in ("[!full_filename!]") do (
        rem If we are here there is a second token, so, there is a special character
        echo Error : Non allowed characters in file name
        endlocal & goto :full_filename_set
    )

    rem Check MAX_PATH (260) limitation
    set "my_temp_file=!cd!\!full_filename!" & if not "!my_temp_file:~260!"=="" (
        echo Error : file name too long
        endlocal & goto :full_filename_set
    )

    rem Check path inclusion, file name correction
    for /f delims^=^ eol^= %%a in ("!full_filename!") do (
        rem Cancel delayed expansion to avoid ! removal during expansion
        endlocal

        rem Until checked, we don't have a valid file
        set "full_filename="

        rem Check we don't have a path 
        if /i not "%%~a"=="%%~nxa" (
            echo Error : Paths are not allowed
            goto :full_filename_set
        )

        rem Check it is not a folder 
        if exist "%%~nxa\" (
            echo Error : Folder with same name present 
            goto :full_filename_set
        )

        rem ASCII 0-31 check. Check file name can be created
        2>nul ( >>"%%~nxa" type nul ) || (
            echo Error : File name is not valid for this file system
            goto :full_filename_set
        )

        rem Ensure it was not a special file name by trying to delete the newly created file
        2>nul ( del /q "%%~nxa" ) || (
            echo Error : Reserved file name used
            goto :full_filename_set
        )

        rem Everithing was OK - We have a file name 
        set "full_filename=%%~nxa"
    )
	
:exe_filename_set
set /p exe_filename=Enter unix filename of your first application (typically you should enter just 'game'):

echo %exe_filename%|findstr /r "^[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789]*$">nul 2>&1
if errorlevel 1 (
    echo Incorrect unix filename! Try again...
    goto exe_filename_set
)

cd ..\depedency\apache_ant\bin
set path=%cd%\..\..\..\depedency\jdk1.8.0_131\bin;%cd%\..\..\..\depedency\mingw\bin;%path%
call ant -v -buildfile "..\..\..\new\ant.xml" new_project -Dunix="%unix_filename%" -Dfull="%full_filename%"
call ant -v -buildfile "..\..\..\new\ant.xml" new_application -Dunix="%exe_filename%" -Dfull="%full_filename%" -Dapp_type="executable" -Dapp_src="%unix_filename%"
cd ..\..\..\..\%unix_filename%
call update_vs_solution.bat
popd
pause