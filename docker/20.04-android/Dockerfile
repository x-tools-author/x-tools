FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get install -y libxcb-xinerama0 libxcb-cursor-dev libudev-dev ninja-build libopengl-dev libglu1-mesa-dev mesa-common-dev
RUN export DEBIAN_FRONTEND=noninteractive && apt-get install -y gcc g++ wget p7zip-full libicu-dev tree grep git libglib2.0-dev
RUN apt-get install -y libxkb* libdbus*
RUN apt-get install -y libegl1-mesa-dev libfontconfig1-dev  libfreetype6-dev  libfreetype-dev unzip
RUN export DEBIAN_FRONTEND=noninteractive && apt-get install -y fuse libfuse2 libfuse-dev kmod

# CMake
RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh
RUN chmod +x cmake-3.30.8-linux-x86_64.sh
RUN ./cmake-3.30.8-linux-x86_64.sh --skip-license --prefix=/usr/local
RUN rm cmake-3.30.8-linux-x86_64.sh

# Copy Qt files from host to container
ARG HOST_QT_ROOT=Qt
ENV QT_ROOT=/opt/qt
# COPY ${HOST_QT_ROOT} ${QT_ROOT}

# Copy Android JDK from host to container
ARG HOST_JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV JAVA_HOME=/opt/jdk
COPY ${HOST_JAVA_HOME} ${JAVA_HOME}

# Copy Android NDK from host to container
# https://github.com/actions/runner-images/blob/main/images/ubuntu/Ubuntu2204-Readme.md
ARG HOST_ANDROID_HOME=/usr/local/lib/android/sdk
ARG HOST_NDK_VERSION=21.4.7075529

ENV ANDROID_HOME=/opt/android/sdk
ENV ANDROID_NDK=${ANDROID_HOME}/ndk/${HOST_NDK_VERSION}
ENV ANDROID_NDK_HOME=${ANDROID_NDK}
ENV ANDROID_NDK_LATEST_HOME=${ANDROID_NDK}
ENV ANDROID_NDK_ROOT=${ANDROID_NDK}
ENV ANDROID_SDK_ROOT=${ANDROID_HOME}

COPY ${HOST_ANDROID_HOME} ${ANDROID_HOME}