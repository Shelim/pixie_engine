pushd %~dp0
cd ../../depedency/apache_ant/bin
set path=%~dp0../../depedency/jdk1.7.0_71/bin;%~dp0../../depedency/mingw/bin;%path%
call ant -v -buildfile "../../../build/windows/ant.xml" %~1 -D%~2 -D%~3 -D%~4 -D%~5 -D%~6 -D%~7 -D%~8
popd