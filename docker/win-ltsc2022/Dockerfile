FROM mcr.microsoft.com/windows/nanoserver:ltsc2022
RUN echo %PATH%

#---------------------------------------------------------------------------------------------------
# Copy Qt to Container
#COPY Qt C:/Qt

#---------------------------------------------------------------------------------------------------
# Install PowerShell
ENV POWERSHELL_URL=https://github.com/PowerShell/PowerShell/releases/download/v7.5.1/PowerShell-7.5.1-win-x64.zip
ENV POWERSHELL_DIR=C:/PowerShell

# Download and install PowerShell
RUN curl.exe -L -o powershell.zip %POWERSHELL_URL% 
RUN tar.exe -xf powershell.zip -C C:\\PowerShell 
RUN del powershell.zip
ENV PATH="%POWERSHELL_DIR%;%PATH%"

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

#---------------------------------------------------------------------------------------------------
# Ninja
ENV NINJA_VERSION=1.12.1
ENV NINJA_URL=https://github.com/ninja-build/ninja/releases/download/v${NINJA_VERSION}/ninja-win.zip
ENV NINJA_DIR=C:/Ninja

# Download and install Ninja
RUN powershell -Command \
    Invoke-WebRequest -Uri %NINJA_URL% -OutFile C:\\ninja.zip; \
    Expand-Archive -Path C:\\ninja.zip -DestinationPath %NINJA_DIR%; \
    Remove-Item -Force C:\\ninja.zip
ENV PATH="%NINJA_DIR%;%PATH%"