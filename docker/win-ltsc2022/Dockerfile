FROM mcr.microsoft.com/windows/nanoserver:ltsc2022

#---------------------------------------------------------------------------------------------------
# Copy Qt to Container
COPY Qt C:/Qt

#---------------------------------------------------------------------------------------------------
# CMake ENV
ENV CMAKE_VERSION=3.30.8
ENV CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-windows-x86_64.zip
ENV CMAKE_DIR=C:/CMake
# Dlownload CMake
RUN powershell -Command \
    Invoke-WebRequest -Uri %CMAKE_URL% -OutFile C:\\cmake.zip; \
    Expand-Archive -Path C:\\cmake.zip -DestinationPath %CMAKE_DIR%; \
    Remove-Item -Force C:\\cmake.zip
ENV PATH="%CMAKE_DIR%/bin;%PATH%"

#---------------------------------------------------------------------------------------------------
# MinGW ENV
ENV MINGW_VERSION=13.2.0
ENV MINGW_URL=https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev1/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z
ENV MINGW_DIR=C:/MinGW

# Download MinGW
RUN powershell -Command \
    Invoke-WebRequest -Uri %MINGW_URL% -OutFile C:\\mingw.7z; \
    Expand-Archive -Path C:\\mingw.7z -DestinationPath %MINGW_DIR%; \
    Remove-Item -Force C:\\mingw.7z
ENV PATH="%MINGW_DIR%/bin;%PATH%"