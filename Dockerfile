# https://github.com/vslotman/docker-aqtinstall/blob/master/Dockerfile

FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get -y install software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa

RUN DEBIAN_FRONTEND=noninteractive apt-get -y install \
    git \
    cmake \
    build-essential \
    libdbus-1-3 \
    libpulse-mainloop-glib0

RUN apt-get install curl
RUN curl -L -O https://download.qt.io/official_releases/online_installers/qt-online-installer-linux-x64-online.run && \
    chmod u+x qt-online-installer-linux-x64-online.run \
    ./qt-online-installer-linux-x64-online.run \
    --accept-licenses \
    --default-answer \
    --confirm-command install \
    qt.qt6.683.linux_gcc_64

RUN pip install -U pip
RUN pip install aqtinstall
#RUN aqt install-qt --outputdir /opt/qt linux desktop 6.8.3 linux_gcc_64 -m qtcharts qtserialbus qtserialport qtwebview
