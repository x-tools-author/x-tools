FROM ubuntu:20.04

RUN export DEBIAN_FRONTEND=noninteractive && apt-get -y update && apt-get install -y \
    libxcb-xinerama0 \
    libxcb-cursor-dev \
    libudev-dev \
    ninja-build \
    libopengl-dev \
    libglu1-mesa-dev \
    mesa-common-dev \
    wget \
    p7zip-full \
    libicu-dev \
    tree \
    grep \
    git \
    libglib2.0-dev \
    libxkb* \
    libdbus* \
    libegl1-mesa-dev \
    libfontconfig1-dev  \
    libfreetype6-dev  \
    libfreetype-dev\
    unzip \
    fuse \
    libfuse2 \
    libfuse-dev \
    kmod

# CMake
RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh
RUN chmod +x cmake-3.30.8-linux-x86_64.sh
RUN ./cmake-3.30.8-linux-x86_64.sh --skip-license --prefix=/usr/local
RUN rm cmake-3.30.8-linux-x86_64.sh

# Copy Qt files from host to container
ENV QT_ROOT=/opt/qt
COPY Qt ${QT_ROOT}

# Copy jdk files from host to container
ARG HOST_JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV JAVA_HOME=${HOST_JAVA_HOME}
COPY jdk ${JAVA_HOME}

# Copy Android SDK files from host to container
ARG HOST_ANDROID_HOME=/usr/local/lib/android/sdk
ARG HOST_ANDROID_NDK=/usr/local/lib/android/sdk/ndk/27.2.12479018
ARG HOST_ANDROID_NDK_HOME=/usr/local/lib/android/sdk/ndk/27.2.12479018
ARG HOST_ANDROID_NDK_LATEST_HOME=/usr/local/lib/android/sdk/ndk/27.2.12479018
ARG HOST_ANDROID_NDK_ROOT=/usr/local/lib/android/sdk/ndk/27.2.12479018
ARG HOST_ANDROID_SDK_ROOT=/usr/local/lib/android/sdk

ENV ANDROID_HOME=${HOST_ANDROID_HOME}
ENV ANDROID_NDK=${HOST_ANDROID_NDK}
ENV ANDROID_NDK_HOME=${HOST_ANDROID_NDK_HOME}
ENV ANDROID_NDK_LATEST_HOME=${HOST_ANDROID_NDK_LATEST_HOME}
ENV ANDROID_NDK_ROOT=${HOST_ANDROID_NDK_ROOT}
ENV ANDROID_SDK_ROOT=${HOST_ANDROID_SDK_ROOT}

COPY sdk/build-tools $ANDROID_HOME/build-tools
COPY sdk/cmdline-tools $ANDROID_HOME/cmdline-tools
COPY sdk/extras $ANDROID_HOME/extras
COPY sdk/licenses $ANDROID_HOME/licenses

# COPY sdk/ndk/27.2.12479018 $ANDROID_HOME/ndk/27.2.12479018

COPY sdk/platforms/android-35 $ANDROID_HOME/platforms/android-35
COPY sdk/platform-tools $ANDROID_HOME/platform-tools
COPY sdk/.knownPackages $ANDROID_HOME/.knownPackages

# Downloading NDK 27.2.12479018 then extracting it to $ANDROID_HOME/ndk/27.2.12479018
RUN wget https://dl.google.com/android/repository/android-ndk-r27c-linux.zip -O /tmp/android-ndk.zip
RUN unzip /tmp/android-ndk.zip -d $ANDROID_HOME/ndk
RUN mv $ANDROID_HOME/ndk/android-ndk-r27 $ANDROID_HOME/ndk/27.2.12479018