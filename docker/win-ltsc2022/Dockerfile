FROM mcr.microsoft.com/windows/nanoserver:ltsc2022
#---------------------------------------------------------------------------------------------------
# Copy File to Container
COPY Qt C:/Qt
RUN dir C:/Qt
RUN echo "Qt directory copied successfully."
ENV PATH=C:\Qt\6.8.3\mingw_64\bin;%PATH%
RUN qmake --version