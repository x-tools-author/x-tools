FROM mcr.microsoft.com/windows/nanoserver:ltsc2022
RUN echo %PATH%

#---------------------------------------------------------------------------------------------------
# Copy File to Container
COPY Qt C:/Qt
COPY MinGW64 C:/MinGW64
COPY CMake3 C:/CMake

ENV PATH="C:/MinGW64/bin;C:/CMake/bin;%PATH%"