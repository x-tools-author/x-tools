FROM ubuntu:22.04:arm64

RUN export DEBIAN_FRONTEND=noninteractive && apt-get -y update && apt-get install -y \
    locales \
    libxcb-xinerama0 \
    libxcb-cursor-dev \
    libudev-dev \
    libudev1 \
    libusb-dev \
    libusb-1.0-0-dev \
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
    libtirpc-dev \
    pkg-config \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Configure system locale to UTF-8 (Chinese + English)
RUN sed -i 's/# zh_CN.UTF-8 UTF-8/zh_CN.UTF-8 UTF-8/' /etc/locale.gen && \
    sed -i 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    locale-gen zh_CN.UTF-8 en_US.UTF-8 && \
    update-locale LANG=zh_CN.UTF-8 LC_ALL=zh_CN.UTF-8 LANGUAGE=zh_CN:zh

ENV LANG=zh_CN.UTF-8
ENV LC_ALL=zh_CN.UTF-8
ENV LANGUAGE=zh_CN:zh

# CMake
# RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh
# RUN chmod +x cmake-3.30.8-linux-x86_64.sh
# RUN ./cmake-3.30.8-linux-x86_64.sh --skip-license --prefix=/usr/local
# RUN rm cmake-3.30.8-linux-x86_64.sh
RUN wget https://cmake.org/files/v4.1/cmake-4.1.4-linux-aarch64.sh
RUN chmod +x cmake-4.1.4-linux-aarch64.sh
RUN ./cmake-4.1.4-linux-aarch64.sh --skip-license --prefix=/usr/local
RUN rm cmake-4.1.4-linux-aarch64.sh

# Copy Qt files from host to container
ENV QT_ROOT=/opt/qt
COPY Qt ${QT_ROOT}

ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV ANDROID_HOME=/usr/local/lib/android/sdk
ENV ANDROID_NDK=${HOST_ANDROID_NDK}/ndk/27.2.12479018
ENV ANDROID_NDK_HOME=${ANDROID_HOME}/ndk/27.2.12479018
ENV ANDROID_NDK_LATEST_HOME=${ANDROID_HOME}/ndk/27.2.12479018
ENV ANDROID_NDK_ROOT=${ANDROID_HOME}/ndk/27.2.12479018
ENV ANDROID_SDK_ROOT=${ANDROID_HOME}
ENV PATH=${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${PATH}

# Make sure the Android SDK directory exists
RUN mkdir -p ${ANDROID_SDK_ROOT}/cmdline-tools

# Download and extract Android Command Line Tools
RUN wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip -O cmdline-tools.zip && \
    unzip cmdline-tools.zip -d ${ANDROID_SDK_ROOT}/cmdline-tools && \
    mv ${ANDROID_SDK_ROOT}/cmdline-tools/cmdline-tools ${ANDROID_SDK_ROOT}/cmdline-tools/latest && \
    rm cmdline-tools.zip

# Accept the licenses then install the required SDK components
RUN yes | sdkmanager --licenses
RUN sdkmanager "platform-tools" "platforms;android-35" "build-tools;33.0.2" "build-tools;35.0.1" "ndk;27.2.12479018"
RUN tree ${ANDROID_SDK_ROOT} -L 2
