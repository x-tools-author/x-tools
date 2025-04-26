FROM mcr.microsoft.com/windows/nanoserver:ltsc2022
RUN echo %PATH%

#---------------------------------------------------------------------------------------------------
# Copy File to Container
# COPY Qt C:/Qt
COPY MinGW64 C:/MinGW64
COPY CMake C:/CMake

ENV PATH="C:/PowerShell;%PATH%"
ENV PWSH="C:/PowerShell/pwsh"

#---------------------------------------------------------------------------------------------------
# CMake ENV
# ENV CMAKE_VERSION=3.30.8
# ENV CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-windows-x86_64.zip
# ENV CMAKE_DIR=C:/CMake
# Dlownload CMake
# RUN %PWSH% -Command \
#     Invoke-WebRequest -Uri %CMAKE_URL% -OutFile C:\\cmake.zip; \
#     Expand-Archive -Path C:\\cmake.zip -DestinationPath %CMAKE_DIR%; \
#     Remove-Item -Force C:\\cmake.zip
# ENV PATH="%CMAKE_DIR%/bin;%PATH%"

#---------------------------------------------------------------------------------------------------
# MinGW ENV
# ENV MINGW_VERSION=13.2.0
# ENV MINGW_URL=https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev1/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z
# ENV MINGW_DIR=C:/MinGW

# Download MinGW
# RUN %PWSH% -Command \
#     Invoke-WebRequest -Uri %MINGW_URL% -OutFile C:\\mingw.7z; \
#     Expand-Archive -Path C:\\mingw.7z -DestinationPath %MINGW_DIR%; \
#     Remove-Item -Force C:\\mingw.7z
# ENV PATH="%MINGW_DIR%/bin;%PATH%"

#---------------------------------------------------------------------------------------------------
# Ninja
# ENV NINJA_VERSION=1.12.1
# ENV NINJA_URL=https://github.com/ninja-build/ninja/releases/download/v${NINJA_VERSION}/ninja-win.zip
# ENV NINJA_DIR=C:/Ninja

# Download and install Ninja
# RUN %PWSH% -Command \
#     Invoke-WebRequest -Uri %NINJA_URL% -OutFile C:\\ninja.zip; \
#     Expand-Archive -Path C:\\ninja.zip -DestinationPath %NINJA_DIR%; \
#     Remove-Item -Force C:\\ninja.zip
# ENV PATH="%NINJA_DIR%;%PATH%"