@echo off
rem *******************************************************************
rem * Simple script that will setup system path for other script
rem * 
rem * REQUIRES:
rem *     - (Nothing)
rem * INPUT:
rem *     (None)
rem * AUTHOR:
rem *     Piotr Kosek <piotr@kosek.com>
rem * LICENSE:
rem *     MIT/X11 License
rem * CHANGELOG:
rem *     0.1 [September, 6th 2017]:
rem *         - Initial version
rem *******************************************************************
rem * CONSTRAINS:
rem *     - It should be the only place to setup path and paths-related environment variables inside project
rem *******************************************************************
rem * Copyright (c) 2017 Piotr Kosek
rem * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
rem * and associated documentation files (the "Software"), to deal in the Software without restriction,
rem * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
rem * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
rem * subject to the following conditions:
rem *
rem * The above copyright notice and this permission notice shall be included in all copies
rem * or substantial portions of the Software.
rem *
rem * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
rem * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
rem * A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
rem * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
rem * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
rem * DEALINGS IN THE SOFTWARE.
rem *******************************************************************

rem * Mark current directory
pushd %~dp0

rem * Jump '\_engine\build\windows' -> '\_engine'
cd ..\..

rem * Set path
set path=%cd%\depedency\other\texlive\bin\win32;%cd%\depedency\other\jdk1.8.0_131\bin;%cd%\depedency\other\llvm\windows\bin;%cd%\depedency\other\python-3.5\windows;%cd%\depedency\other\mingw\windows\bin;%cd%\depedency\other\apache_ant\bin;%cd%\depedency\other\doxygen\windows;%cd%\depedency\other\dot\windows;%cd%\depedency\other\windbg
set JAVA_HOME=%cd%\depedency\other\jdk1.8.0_131\windows
set PYTHONPATH=%cd%\depedency\other\python-3.5\windows;%cd%\depedency\other\python-3.5\windows\Lib;%cd%\depedency\other\python-3.5\windows\Lib\site-packages
set PYTHONHOME=%cd%\depedency\other\python-3.5\windows
set PYTHON_HOME=%cd%\depedency\other\python-3.5\windows

rem * Get back current directory
popd