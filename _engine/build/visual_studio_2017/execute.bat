pushd %~dp0
cd ../../depedency/apache_ant/bin
set path=%~dp0../../depedency/jdk1.7.0_71/bin;%~dp0../../depedency/mingw/bin;%path%
call ant -v -buildfile "../../../build/visual_studio_2017/ant.xml" update_vs_solution -Dapp_unix_name="%~1"
popd