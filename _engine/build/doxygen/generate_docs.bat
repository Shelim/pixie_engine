pushd %~dp0
rmdir /s /q ..\..\_build\doxygen
mkdir ..\..\_build\doxygen
cd windows
set current_directory=%CD%
"../../../depedency/doxygen/doxygen.exe" doxyfile
popd