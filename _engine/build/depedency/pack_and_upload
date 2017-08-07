#!/bin/bash

cd "$(dirname "$0")"

echo "Enter FTP host:"
read ftp_server

echo "Enter FTP login:"
read ftp_login

echo "Enter FTP password:"
read -s ftp_password

cd ../../depedency/apache_ant/bin

./ant -v -buildfile "../../../build/depedency/ant.xml" pack_and_upload -Dftp_server="%ftp_server%" -Dftp_login="%ftp_login%" -Dftp_password="%ftp_password%"

echo "All done!"
