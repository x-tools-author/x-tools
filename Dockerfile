# https://github.com/vslotman/docker-aqtinstall/blob/master/Dockerfile

FROM ubuntu:20.04

RUN apt-get -y update
RUN apt-get -y install software-properties-common
RUN add-apt-repository ppa:deadsnakes/ppa

RUN DEBIAN_FRONTEND=noninteractive apt-get -y install \
    git \
    cmake \
    python3.13 \
    python3-pip \
    build-essential \
    libdbus-1-3 \
    libpulse-mainloop-glib0

RUN ls /usr/bin/ -l | grep python
RUN ls /usr/bin/ -l | grep pip
RUN ln -sf /usr/bin/python3.13 /usr/bin/python3
RUN which python3
RUN python3 --version

RUN pip3 install -U pip3
RUN pip3 install aqtinstall
#RUN aqt install-qt --outputdir /opt/qt linux desktop 6.8.3 linux_gcc_64 -m qtcharts qtserialbus qtserialport qtwebview
