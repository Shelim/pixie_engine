rem @echo off
pushd %~dp0

set unix_filename=example_app
set full_filename="My Example Application"
set exe_filename=game

set /p unix_filename=Enter unix filename of your project (lowercase, no spaces nor special characters):
set /p full_filename=Enter full name of your project (should be a valid filename!):
set /p exe_filename=Enter unix filename of your first application (typically you should enter just 'game'):

cd ../depedency/apache_ant/bin
set path=%~dp0../depedency/jdk1.7.0_71/bin;%~dp0../depedency/mingw/bin;%path%
echo on
call ant -v -buildfile "../../../new/ant.xml" new_project -Dunix="%unix_filename%" -Dfull="%full_filename%"
call ant -v -buildfile "../../../new/ant.xml" new_application -Dunix="%exe_filename%" -Dfull="%full_filename%" -Dapp_type="executable" -Dapp_src="%unix_filename%"
cd ../../../../%unix_filename%
call update_vs_solution.bat
popd
pause