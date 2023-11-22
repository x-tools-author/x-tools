#!/bin/sh
apt-get update --fix-missing -y
apt install qt5-base* -y
apt install libqt5charts5* -y
apt install libqt5serialbus5* -y
apt install libqt5websockets5* -y
apt install libqt5serialport5* -y
mkdir buildqt5
cd buildqt5
cmake -DQT_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5 -DQt5_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5 -DCMAKE_BUILD_TYPE="Release" ../ && make