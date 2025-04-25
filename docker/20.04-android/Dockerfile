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

#---------------------------------------------------------------------------------------------------
# Qt
ARG QT_BAST_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.linux_gcc_64/6.8.3-0-202503201345qtbase-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_CHARTS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtcharts.linux_gcc_64/6.8.3-0-202503201345qtcharts-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_SERIAL_PORT_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtserialport.linux_gcc_64/6.8.3-0-202503201345qtserialport-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_SERIAL_BUS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtserialbus.linux_gcc_64/6.8.3-0-202503201345qtserialbus-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_WEB_SOCKETS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtwebsockets.linux_gcc_64/6.8.3-0-202503201345qtwebsockets-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_SVG_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.linux_gcc_64/6.8.3-0-202503201345qtsvg-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_TOOLS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.linux_gcc_64/6.8.3-0-202503201345qttools-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ARG QT_ICU_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.linux_gcc_64/6.8.3-0-202503201345icu-linux-Rhel8.6-x86_64.7z
ARG QT_PREFIX_GCC=/opt/qt/gcc_64

# Extract Qt
RUN mkdir -p ${QT_PREFIX_GCC}
RUN wget ${QT_BAST_URL} -O qtbase.7z
RUN wget ${QT_CHARTS_URL} -O qtcharts.7z
RUN wget ${QT_SERIAL_PORT_URL} -O qtserialport.7z
RUN wget ${QT_SERIAL_BUS_URL} -O qtserialbus.7z
RUN wget ${QT_WEB_SOCKETS_URL} -O qtwebsockets.7z
RUN wget ${QT_SVG_URL} -O qtsvg.7z
RUN wget ${QT_TOOLS_URL} -O qttools.7z
RUN wget ${QT_ICU_URL} -O icu.7z

RUN 7z x qtbase.7z -o${QT_PREFIX_GCC} && rm qtbase.7z
RUN 7z x qtcharts.7z -o${QT_PREFIX_GCC} && rm qtcharts.7z
RUN 7z x qtserialport.7z -o${QT_PREFIX_GCC} && rm qtserialport.7z
RUN 7z x qtserialbus.7z -o${QT_PREFIX_GCC} && rm qtserialbus.7z
RUN 7z x qtwebsockets.7z -o${QT_PREFIX_GCC} && rm qtwebsockets.7z
RUN 7z x qtsvg.7z -o${QT_PREFIX_GCC} && rm qtsvg.7z
RUN 7z x qttools.7z -o${QT_PREFIX_GCC} && rm qttools.7z
RUN 7z x icu.7z -o${QT_PREFIX_GCC}/lib && rm icu.7z

# Set environment variables for Qt
ENV LD_LIBRARY_PATH ${QT_PREFIX_GCC}/lib:$LD_LIBRARY_PATH
ENV PATH ${QT_PREFIX_GCC}/bin:$PATH
ENV QT_PLUGIN_PATH ${QT_PREFIX_GCC}/plugins/
ENV QML_IMPORT_PATH ${QT_PREFIX_GCC}/qml/
ENV QML2_IMPORT_PATH ${QT_PREFIX_GCC}/qml/

#---------------------------------------------------------------------------------------------------
# Qt Android
ARG QT_BAST_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.android_armv7/6.8.3-0-202503201334qtbase-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_CHARTS_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.addons.qtcharts.android_armv7/6.8.3-0-202503201334qtcharts-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_SERIAL_PORT_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.addons.qtserialport.android_armv7/6.8.3-0-202503201334qtserialport-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_SERIAL_BUS_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.addons.qtserialbus.android_armv7/6.8.3-0-202503201334qtserialbus-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_WEB_SOCKETS_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.addons.qtwebsockets.android_armv7/6.8.3-0-202503201334qtwebsockets-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_SVG_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.android_armv7/6.8.3-0-202503201334qtsvg-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_TOOLS_ARMv7_URL=https://download.qt.io/online/qtsdkrepository/all_os/android/qt6_683/qt6_683_armv7/qt.qt6.683.android_armv7/6.8.3-0-202503201334qttools-Windows-Windows_10_22H2-Clang-Android-Android_ANY-ARMv7.7z
ARG QT_PREFIX_ARMv7=/opt/qt/android_armv7

# Extract Qt
RUN mkdir -p ${QT_PREFIX_ARMv7}
RUN wget ${QT_BAST_ARMv7_URL} -O qtbase.7z
RUN wget ${QT_CHARTS_ARMv7_URL} -O qtcharts.7z
RUN wget ${QT_SERIAL_PORT_ARMv7_URL} -O qtserialport.7z
RUN wget ${QT_SERIAL_BUS_ARMv7_URL} -O qtserialbus.7z
RUN wget ${QT_WEB_SOCKETS_ARMv7_URL} -O qtwebsockets.7z
RUN wget ${QT_SVG_ARMv7_URL} -O qtsvg.7z
RUN wget ${QT_TOOLS_ARMv7_URL} -O qttools.7z

RUN 7z x qtbase.7z -o${QT_PREFIX_ARMv7} && rm qtbase.7z
RUN 7z x qtcharts.7z -o${QT_PREFIX_ARMv7} && rm qtcharts.7z
RUN 7z x qtserialport.7z -o${QT_PREFIX_ARMv7} && rm qtserialport.7z
RUN 7z x qtserialbus.7z -o${QT_PREFIX_ARMv7} && rm qtserialbus.7z
RUN 7z x qtwebsockets.7z -o${QT_PREFIX_ARMv7} && rm qtwebsockets.7z
RUN 7z x qtsvg.7z -o${QT_PREFIX_ARMv7} && rm qtsvg.7z
RUN 7z x qttools.7z -o${QT_PREFIX_ARMv7} && rm qttools.7z

#---------------------------------------------------------------------------------------------------
# JDK 17
ARG JDK17_URL=https://download.oracle.com/java/17/latest/jdk-17_linux-x64_bin.tar.gz
ARG JDK17_PREFIX=/opt/jdk17
RUN wget ${JDK17_URL} -O jdk17.tar.gz
RUN mkdir -p ${JDK17_PREFIX}
RUN tar -xzf jdk17.tar.gz -C ${JDK17_PREFIX} --strip-components=1
RUN rm jdk17.tar.gz

ENV JAVA_HOME ${JDK17_PREFIX}

#---------------------------------------------------------------------------------------------------
# NDK 25
ARG NDK25_URL=https://dl.google.com/android/repository/android-ndk-r25-linux.zip
ARG NDK25_PREFIX=/opt/android-ndk-r25
RUN wget ${NDK25_URL} -O android-ndk-r25-linux.zip
RUN mkdir -p ${NDK25_PREFIX}
RUN unzip android-ndk-r25-linux.zip -d ${NDK25_PREFIX} && rm android-ndk-r25-linux.zip

ENV NDK_HOME ${NDK25_PREFIX}
ENV ANDROID_NDK_HOME ${NDK25_PREFIX}
ENV PATH ${NDK25_PREFIX}/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
ENV PATH ${NDK25_PREFIX}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin:$PATH

#---------------------------------------------------------------------------------------------------
# SDK 34
ARG SDK34_URL=https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip
ARG SDK34_PREFIX=/opt/android-sdk
RUN wget ${SDK34_URL} -O commandlinetools-linux.zip
RUN mkdir -p ${SDK34_PREFIX}/cmdline-tools
RUN unzip commandlinetools-linux.zip -d ${SDK34_PREFIX}/cmdline-tools && rm commandlinetools-linux.zip
RUN mv ${SDK34_PREFIX}/cmdline-tools/cmdline-tools ${SDK34_PREFIX}/cmdline-tools/latest
RUN yes | ${SDK34_PREFIX}/cmdline-tools/latest/bin/sdkmanager --sdk_root=${SDK34_PREFIX} --licenses

ENV ANDROID_HOME ${SDK34_PREFIX}
ENV ANDROID_SDK_ROOT ${SDK34_PREFIX}