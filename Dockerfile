FROM ubuntu:20.04

RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install software-properties-common
RUN add-apt-repository -y ppa:deadsnakes/ppa
RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    git \
    cmake \
    python3.13 \
    python3-pip \
    build-essential \
    libdbus-1-3 \
    libpulse-mainloop-glib0

RUN which pip
RUN python3 --version
RUN ls -l /usr/bin/ | grep python
RUN ls -l /usr/bin/ | grep pip
RUN ln -sf /usr/bin/python3.13 /usr/bin/python3
RUN pip3 install -U pip3
RUN pip3 install aqtinstall

ARG QT=6.8.3
ARG QT_MODULES="qtcharts qtserialbus qtserialport qtwebsockets"
ARG QT_HOST=linux
ARG QT_TARGET=desktop
ARG QT_ARCH= linux_gcc_64
RUN aqt install-qt --outputdir /opt/qt ${QT_HOST} ${QT_TARGET} ${QT} ${QT_ARCH} -m ${QT_MODULES}

ENV PATH /opt/qt/${QT}/gcc_64/bin:$PATH
ENV QT_PLUGIN_PATH /opt/qt/${QT}/gcc_64/plugins/
ENV QML_IMPORT_PATH /opt/qt/${QT}/gcc_64/qml/
ENV QML2_IMPORT_PATH /opt/qt/${QT}/gcc_64/qml/