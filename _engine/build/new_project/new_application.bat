@echo off
pushd %~dp0
:choose_type
echo Enter type of your application:
echo 1 - Executable
echo 2 - Shared library
echo 3 - Test framework
choice /N /C:123 /M "Choose one"
if errorlevel ==3 goto type_test
if errorlevel ==2 goto type_shared
if errorlevel ==1 goto type_exe
echo Wrong answer. Please try again.
goto choose_type
:type_exe
set app_type=executable
goto after_type
:type_shared
set app_type=shared_library
goto after_type
:type_test
set app_type=test
goto after_type
:after_type
set /p unix_filename=Enter unix filename of your new application (lowercase, no spaces nor special characters): 
set /p full_filename=Enter full name of your application (should be a valid filename!): 
cd ../../depedency/apache_ant/bin
set path=%~dp0../../depedency/jdk1.7.0_71/bin;%~dp0../../depedency/mingw/bin;%path%
echo on
call ant -v -buildfile "../../../build/new_project/ant.xml" new_application -Dunix="%unix_filename%" -Dfull="%full_filename%" -Dapp_type="%app_type%" -Dapp_src="%~1"
cd ../../../../%~1
call update_vs_solution.bat
popd
pause