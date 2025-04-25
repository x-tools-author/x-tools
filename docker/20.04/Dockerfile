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
    kmod \
    openjdk-17-jdk-headless \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

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

ENV ANDROID_HOME=/usr/local/lib/android/sdk
ENV ANDROID_NDK=${HOST_ANDROID_NDK}
ENV ANDROID_NDK_HOME=${HOST_ANDROID_NDK_HOME}
ENV ANDROID_NDK_LATEST_HOME=${HOST_ANDROID_NDK_LATEST_HOME}
ENV ANDROID_NDK_ROOT=${HOST_ANDROID_NDK_ROOT}
ENV ANDROID_SDK_ROOT=${HOST_ANDROID_SDK_ROOT}
ENV PATH=${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${PATH}

# 创建 SDK 目录
RUN mkdir -p ${ANDROID_SDK_ROOT}/cmdline-tools

# 下载并解压 Android Command Line Tools
RUN wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip -O cmdline-tools.zip && \
    unzip cmdline-tools.zip -d ${ANDROID_SDK_ROOT}/cmdline-tools && \
    mv ${ANDROID_SDK_ROOT}/cmdline-tools/cmdline-tools ${ANDROID_SDK_ROOT}/cmdline-tools/latest && \
    rm cmdline-tools.zip

# 接受所有 SDK 许可协议
RUN yes | sdkmanager --licenses

# 安装 SDK 和 NDK
RUN sdkmanager "platform-tools" "platforms;android-35" "build-tools;33.0.2" "ndk;27.2.12479018"

# 验证安装
RUN tree ${ANDROID_SDK_ROOT} -L 2