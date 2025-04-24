# https://github.com/vslotman/docker-aqtinstall/blob/master/Dockerfile

FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get install -y libxcb-xinerama0 libxcb-cursor-dev libudev-dev ninja-build
RUN apt-get install -y gcc g++ wget

# CMake
RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh 
RUN chmod +x cmake-3.30.8-linux-x86_64.sh 