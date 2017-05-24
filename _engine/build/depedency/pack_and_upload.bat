@echo off
pushd %~dp0
set /p ftp_server=Enter FTP host: 
set /p ftp_login=Enter FTP login: 
set "psCommand=powershell -Command "$pword = read-host 'Enter FTP password' -AsSecureString ; ^
    $BSTR=[System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($pword); ^
        [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($BSTR)""
for /f "usebackq delims=" %%p in (`%psCommand%`) do set ftp_password=%%p
cd ../../depedency/apache_ant/bin
set path=%~dp0../../depedency/jdk1.7.0_71/bin;%~dp0../../depedency/mingw/bin;%path%
echo on
call ant -v -buildfile "../../../build/depedency/ant.xml" pack_and_upload -Dftp_server="%ftp_server%" -Dftp_login="%ftp_login%" -Dftp_password="%ftp_password%"
popd
echo ALl done!
pause