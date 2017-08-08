pushd %~dp0
rmdir /s /q ..\..\_build\doxygen
mkdir ..\..\_build\doxygen
rmdir /s /q ..\..\docs
mkdir ..\..\docs
cd windows
set current_directory=%CD%
"../../../depedency/doxygen/windows/doxygen.exe" doxyfile
popd