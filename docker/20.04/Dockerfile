FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get install -y libxcb-xinerama0 libxcb-cursor-dev libudev-dev ninja-build libopengl-dev libglu1-mesa-dev mesa-common-dev
RUN apt-get install -y gcc g++ wget p7zip-full

# CMake
RUN wget https://cmake.org/files/v3.30/cmake-3.30.8-linux-x86_64.sh
RUN chmod +x cmake-3.30.8-linux-x86_64.sh
RUN ./cmake-3.30.8-linux-x86_64.sh --skip-license --prefix=/usr/local
RUN rm cmake-3.30.8-linux-x86_64.sh

# Qt
ENV QT_BAST_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.linux_gcc_64/6.8.3-0-202503201345qtbase-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ENV QT_CHARTS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtcharts.linux_gcc_64/6.8.3-0-202503201345qtcharts-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ENV QT_SERIAL_PORT_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtserialport.linux_gcc_64/6.8.3-0-202503201345qtserialport-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ENV QT_SERIAL_BUS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtserialbus.linux_gcc_64/6.8.3-0-202503201345qtserialbus-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ENV QT_WEB_SOCKETS_URL=https://download.qt.io/online/qtsdkrepository/linux_x64/desktop/qt6_683/qt6_683/qt.qt6.683.addons.qtwebsockets.linux_gcc_64/6.8.3-0-202503201345qtwebsockets-Linux-RHEL_8_10-GCC-Linux-RHEL_8_10-X86_64.7z
ENV QT_PREFIX=/opt/qt

# Extract Qt
RUN mkdir -p ${QT_PREFIX}
RUN wget ${QT_BAST_URL} -O qtbase.7z
RUN wget ${QT_CHARTS_URL} -O qtcharts.7z
RUN wget ${QT_SERIAL_PORT_URL} -O qtserialport.7z
RUN wget ${QT_SERIAL_BUS_URL} -O qtserialbus.7z
RUN wget ${QT_WEB_SOCKETS_URL} -O qtwebsockets.7z
RUN 7z x qtbase.7z -o${QT_PREFIX} && rm qtbase.7z
RUN 7z x qtcharts.7z -o${QT_PREFIX} && rm qtcharts.7z
RUN 7z x qtserialport.7z -o${QT_PREFIX} && rm qtserialport.7z
RUN 7z x qtserialbus.7z -o${QT_PREFIX} && rm qtserialbus.7z
RUN 7z x qtwebsockets.7z -o${QT_PREFIX} && rm qtwebsockets.7z

# Set environment variables for Qt
ENV LD_LIBRARY_PATH ${QT_PREFIX}/lib:$LD_LIBRARY_PATH
ENV PATH ${QT_PREFIX}/bin:$PATH
ENV QT_PLUGIN_PATH ${QT_PREFIX}/plugins/
ENV QML_IMPORT_PATH ${QT_PREFIX}/qml/
ENV QML2_IMPORT_PATH ${QT_PREFIX}/qml/