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

# 2025-04-25T05:36:02.9638620Z ├── build-tools
# 2025-04-25T05:36:02.9640011Z │   ├── 31.0.0
# 2025-04-25T05:36:02.9641165Z │   ├── 32.0.0
# 2025-04-25T05:36:02.9642678Z │   ├── 33.0.0
# 2025-04-25T05:36:02.9643830Z │   ├── 33.0.1
# 2025-04-25T05:36:02.9644989Z │   ├── 33.0.2
# 2025-04-25T05:36:02.9646113Z │   ├── 33.0.3
# 2025-04-25T05:36:02.9646953Z │   ├── 34.0.0
# 2025-04-25T05:36:02.9647796Z │   ├── 35.0.0
# 2025-04-25T05:36:02.9648635Z │   ├── 35.0.1
# 2025-04-25T05:36:02.9649480Z │   └── 36.0.0
# 2025-04-25T05:36:02.9650326Z ├── cmake
# 2025-04-25T05:36:02.9651125Z │   ├── 3.18.1
# 2025-04-25T05:36:02.9652158Z │   ├── 3.22.1
# 2025-04-25T05:36:02.9652991Z │   └── 3.31.5
# 2025-04-25T05:36:02.9653858Z ├── cmdline-tools
# 2025-04-25T05:36:02.9654745Z │   └── latest
# 2025-04-25T05:36:02.9655564Z ├── extras
# 2025-04-25T05:36:02.9656346Z │   ├── android
# 2025-04-25T05:36:02.9657176Z │   └── google
# 2025-04-25T05:36:02.9657970Z ├── licenses
# 2025-04-25T05:36:02.9658808Z │   └── android-sdk-license
# 2025-04-25T05:36:02.9659767Z ├── ndk
# 2025-04-25T05:36:02.9660513Z │   ├── 26.3.11579264
# 2025-04-25T05:36:02.9661365Z │   └── 27.2.12479018
# 2025-04-25T05:36:02.9662450Z ├── packages-list.txt
# 2025-04-25T05:36:02.9663415Z ├── platform-tools
# 2025-04-25T05:36:02.9664261Z │   ├── NOTICE.txt
# 2025-04-25T05:36:02.9665080Z │   ├── adb
# 2025-04-25T05:36:02.9665888Z │   ├── etc1tool
# 2025-04-25T05:36:02.9666692Z │   ├── fastboot
# 2025-04-25T05:36:02.9667494Z │   ├── hprof-conv
# 2025-04-25T05:36:02.9668297Z │   ├── lib64
# 2025-04-25T05:36:02.9669059Z │   ├── make_f2fs
# 2025-04-25T05:36:02.9669891Z │   ├── make_f2fs_casefold
# 2025-04-25T05:36:02.9670812Z │   ├── mke2fs
# 2025-04-25T05:36:02.9671773Z │   ├── mke2fs.conf
# 2025-04-25T05:36:02.9672616Z │   ├── package.xml
# 2025-04-25T05:36:02.9673481Z │   ├── source.properties
# 2025-04-25T05:36:02.9674404Z │   └── sqlite3
# 2025-04-25T05:36:02.9675190Z └── platforms
# 2025-04-25T05:36:02.9676022Z     ├── android-31
# 2025-04-25T05:36:02.9676842Z     ├── android-32
# 2025-04-25T05:36:02.9677648Z     ├── android-33
# 2025-04-25T05:36:02.9678478Z     ├── android-33-ext4
# 2025-04-25T05:36:02.9679376Z     ├── android-33-ext5
# 2025-04-25T05:36:02.9680246Z     ├── android-34
# 2025-04-25T05:36:02.9681086Z     ├── android-34-ext10
# 2025-04-25T05:36:02.9682406Z     ├── android-34-ext11
# 2025-04-25T05:36:02.9683361Z     ├── android-34-ext12
# 2025-04-25T05:36:02.9684329Z     ├── android-34-ext8
# 2025-04-25T05:36:02.9685207Z     ├── android-35
# 2025-04-25T05:36:02.9686045Z     ├── android-35-ext14
# 2025-04-25T05:36:02.9686990Z     ├── android-35-ext15
# 2025-04-25T05:36:02.9687897Z     └── android-36

# 分开复制，避免文件过大
# failed to walk /var/lib/docker/tmp/buildkit-mount...: no such file or directory
COPY ${HOST_ANDROID_HOME}/build-tools ${ANDROID_HOME}/build-tools
COPY ${HOST_ANDROID_HOME}/cmake ${ANDROID_HOME}/cmake
COPY ${HOST_ANDROID_HOME}/cmdline-tools ${ANDROID_HOME}/cmdline-tools
COPY ${HOST_ANDROID_HOME}/extras ${ANDROID_HOME}/extras
COPY ${HOST_ANDROID_HOME}/licenses ${ANDROID_HOME}/licenses
COPY ${HOST_ANDROID_HOME}/ndk/${HOST_NDK_VERSION} ${ANDROID_HOME}/ndk/${HOST_NDK_VERSION}
COPY ${HOST_ANDROID_HOME}/packages-list.txt ${ANDROID_HOME}/packages-list.txt
COPY ${HOST_ANDROID_HOME}/platform-tools ${ANDROID_HOME}/platform-tools
COPY ${HOST_ANDROID_HOME}/platforms ${ANDROID_HOME}/platforms