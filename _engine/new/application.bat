@echo off
pushd %~dp0

if "%~1"=="" goto no_app_src
set app_name="%~1"
goto choose_type 

:no_app_src
set /p app_name=Enter project unix name you wish to add new application to:
IF EXIST ..\..\%app_name% goto choose_type
echo This project seems not to exists, choose existing one!
goto no_app_src

:choose_type
echo Do you wish to create sample application, or start with blank project?
echo 1 - Blank application
echo 2 - Launcher
echo 3 - Editor
echo 4 - Engine Tester
echo 5 - Windows Icon Provider (Shared lib)
choice /N /C:12345 /M "Choose one"
if errorlevel ==5 goto type_icon_provider
if errorlevel ==4 goto type_tester
if errorlevel ==3 goto type_editor
if errorlevel ==2 goto type_launcher
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

:type_icon_provider
set app_predefined=icon_provider
set app_type=shared_library
goto after_type
:type_tester
set app_predefined=tester
set app_type=test
goto after_type
:type_editor
set app_predefined=editor
set app_type=executable
goto after_type
:type_launcher
set app_predefined=launcher
set app_type=executable
goto after_type
:type_exe
set app_predefined=executable
set app_type=executable
goto after_type
:type_shared
set app_predefined=shared_library
set app_type=shared_library
goto after_type
:type_test
set app_predefined=test
set app_type=test
goto after_type
:after_type

if %app_type%==executable (
:choose_launch
echo Do you wish to make this app launch as default?
echo 1 - Yes
echo 2 - No
choice /N /C:12 /M "Choose one"
if errorlevel ==2 goto launch_no
if errorlevel ==1 goto launch_yes
echo Wrong answer. Please try again.
goto choose_launch
:launch_yes
set app_make_default=true
goto after_launch
:launch_no
set app_make_default=false
goto after_launch
)

:after_launch
:set_unix_filename
set /p unix_filename=Enter unix filename of your new application (lowercase, no spaces nor special characters): 

IF NOT EXIST ..\..\%app_name%\manifest\%unix_filename% goto set_full_filename
echo This project does already exists, choose existing one!
goto set_unix_filename
:set_full_filename
set /p full_filename=Enter full name of your application (should be a valid filename!): 
cd ..\depedency\apache_ant\bin
set path=%cd%\..\..\..\depedency\jdk1.8.0_131\bin;%cd%\..\..\..\depedency\mingw\bin;%path%
echo on
call ant -v -buildfile "..\..\..\new\ant.xml" new_application -Dunix="%unix_filename%" -Dfull="%full_filename%" -Dapp_type="%app_type%" -Dapp_src="%app_name%" -Dapp_predefined="%app_predefined%" -Dapp_make_default="%app_make_default%"
cd ..\..\..\..\%app_name%
call update_vs_solution.bat

popd
pause