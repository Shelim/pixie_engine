pushd %~dp0
echo on
cd ..\..\..\depedency\apache_ant\bin
set path=%cd%\..\..\..\depedency\jdk1.8.0_131\bin;%cd%\..\..\..\depedency\mingw\bin;%path%
call ant -v -buildfile "..\..\..\build\windows\project\visual_studio_2015\ant.xml" update_vs_solution -Dapp_unix_name="%~1"
popd