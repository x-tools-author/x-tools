#!/bin/sh
apt-get update --fix-missing -y
apt-get install libgl1-mesa-dev -y
apt-get install qt6-base-dev -y
apt-get install libqt6charts6* -y
apt-get install libqt6serialbus6* -y
apt-get install libqt6websockets6* -y
apt-get install libqt6serialport6* -y
mkdir buildqt6
cd buildqt6
cmake -DQT_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6 -DQt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6 -DCMAKE_BUILD_TYPE="Release" ../ && make