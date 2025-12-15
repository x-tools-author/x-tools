/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftp.h"
#include "tftp_p.h"

#include <fcntl.h>
#include <string.h>
#include <QCollator>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QMetaType>
#include <QMutex>
#include <QThread>
#include <QtEndian>

Tftp::Tftp(QObject *parent)
    : QThread{parent}
{
    x_d_ptr = new TftpPrivate(this);

    timeoutTimer = new QTimer();
    timeoutTimer->setInterval(1000);
    timeoutTimer->setSingleShot(true);
    readBuf.clear();
    connect(timeoutTimer, &QTimer::timeout, this, [&]() {
        if (--retryCount <= 0) {
            ////print << "\ntimeout\n";
            clearStatus();
            sendErrorCode(EBADOP);
            retryCount = 3;
        } else {
            // 重新发送上一个数据包
            if (sendBuf) {
                int size = (int) (sptr - sendBuf);
                QByteArray data(sendBuf, size);
                udpSocket->writeDatagram(data, destAddress, destPort);
            }
            // 重新启动定时器
            if (timeoutTimer) {
                timeoutTimer->start();
            }
        }
    });

    sendBuf = new char[BUFFER_SIZE];
    memset(sendBuf, 0, BUFFER_SIZE);
    sptr = sendBuf;
    setWorkPathSlots(QDir::currentPath());

    if (!udpSocket) {
        udpSocket = new QUdpSocket;
        udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 65536);

        connect(udpSocket, &QUdpSocket::readyRead, this, &Tftp::readPendingDatagramsSlots);
    }
}

Tftp::~Tftp()
{
    isRunningFlag = false;
    if (this->isRunning()) {
        this->exit(0);
    }
    clearStatus();
    if (udpSocket) {
        QObject::disconnect(udpSocket,
                            &QUdpSocket::readyRead,
                            this,
                            &Tftp::readPendingDatagramsSlots);
        udpSocket->deleteLater();
        udpSocket = nullptr;
    }
    if (file) {
        file->close();
        file->deleteLater();
        file = nullptr;
    }
    if (timeoutTimer) {
        timeoutTimer->stop();
        timeoutTimer->deleteLater();
        timeoutTimer = nullptr;
    }
    if (sendBuf) {
        delete[] sendBuf;
        sendBuf = nullptr;
    }
}

void Tftp::run()
{
    on_recvfrom();
}

void Tftp::setWorkPathSlots(QString path)
{
    curFilePath = path;
    if (!curFilePath.endsWith('/')) {
        curFilePath += '/';
    }
}

void Tftp::readPendingDatagramsSlots()
{
    if (!this->isRunning())
        this->start();
    this->dataAvalivabe = true;
}
void Tftp::printProcess(int maxValue, int curValue)
{
    if (curValue <= 0 || maxValue <= 0)
        return;
    int progress = (((float) curValue / (float) maxValue)) * 100;
    if (progress != progress_bak) {
        progress_bak = progress;
        emit transferProgressSignal(curFilePath + pre_filePath, curValue, maxValue, 0);
        ////print << __FUNCTION__ << progress;
    }
}

void Tftp::setPortSlots(int port)
{
    this->port = port;
    udpSocket->disconnectFromHost();
    if (udpSocket->bind(QHostAddress::Any, this->port) == false) {
        QString msg = "bind port 69 failed";
        ////print << msg;
        emit tftpErrorSignal(msg);
    } else {
        ////print << "bind port 69 success";
        initFlag = true;
    }
}

void Tftp::on_recvfrom()
{
    while (isRunningFlag) {
        if (isClientMode) {
            processClientDatagram();
        } else {
            if (!initFlag) {
                if (udpSocket->bind(QHostAddress::Any, this->port) == false) {
                    //print << "bind port 69 failed";
                    QString msg = "bind port 69 failed";
                    //print << msg;
                    emit tftpErrorSignal(msg);
                    break;
                } else {
                    //print << "bind port 69 success";
                    initFlag = true;
                }
            }
            //         if(this->dataAvalivabe==false)
            //             continue;
            uint16_t code, cmd = 0;
            char *ptr;

            int readLen = 0, pos = 0;
            uint16_t index = 0;
            bool ok = false;
            retryCount = 3;
            uint32_t readbytes = 0;
            uint16_t block_current_index = 0;
            while (udpSocket->hasPendingDatagrams()) {
                datagram = udpSocket->receiveDatagram();
                if (!datagram.isValid()) {
                    //print << __LINE__;
                    continue;
                }
                destAddress = datagram.senderAddress();
                destPort = datagram.senderPort();
                //print << __LINE__ << destAddress << destPort;
                readBuf.clear();
                readBuf = datagram.data();
                datagram.clear();
                ptr = readBuf.data();
                code = ((ptr[0] >> 8) & 0xff) | (ptr[1] & 0xff);
                pos += 2;
                //print << __LINE__ << code;

                switch (code) {
                case _RRQ:
                case _WRQ:
                    pre_filePath = &ptr[pos];
                    //print << "file name is " + curFilePath + pre_filePath;
                    pos += pre_filePath.length() + 1;
                    if (code == _WRQ) {
                        QFileInfo info(curFilePath + pre_filePath);

                        if (info.isDir()) {
                            sendErrorCode(EACCESS);
                            clearStatus();
                            break;
                        }
                        file = new QFile(curFilePath + pre_filePath);
                        file->open(QIODevice::WriteOnly | QIODevice::Truncate);
                        if (!file) {
                            sendErrorCode(ENOTFOUND);
                            clearStatus();
                            break;
                        }
                    } else {
                        /*获取文件信息，把信息放到s_buf中*/
                        QFileInfo info(curFilePath + pre_filePath);

                        if (info.isDir()) {
                            sendErrorCode(ENOTFOUND);
                            clearStatus();
                            break;
                        }
                        file = new QFile(curFilePath + pre_filePath);
                        file->open(QIODevice::ReadOnly);
                        if (!file) {
                            sendErrorCode(ENOTFOUND);
                            clearStatus();
                            break;
                        }
                    }

                    while (1) {
                        if (pos >= readBuf.count())
                            break;
                        QString tmp = "";
                        tmp = &ptr[pos];
                        if (tmp == "octet") {
                            transportType = OCTET;
                            pos += tmp.length() + 1;
                        } else if (tmp == "tsize") {
                            pos += (int) strlen("tsize") + 1;
                            tmp = "";
                            tmp = &ptr[pos];
                            file_size = tmp.toUInt(&ok, 10);
                            if (!ok) {
                                sendErrorCode(EUNDEF);
                                if (file)
                                    file->close();
                                clearStatus();
                                return;
                            }
                            totalSize = file_size;
                            char tt[256] = {0};
                            pos += sprintf(tt, "%d", file_size) + 1;
                            //print << __LINE__ << file_size;
                        } else if (tmp == "timeout") {
                            pos += (int) strlen("timeout") + 1;
                            tmp = "";
                            tmp = &ptr[pos];
                            timeout = tmp.toUInt(&ok, 10);
                            if (!ok) {
                                sendErrorCode(EUNDEF);
                                if (file)
                                    file->close();
                                clearStatus();
                                return;
                            }
                            char tt[256] = {0};
                            pos += sprintf(tt, "%d", timeout) + 1;
                            //print << QString::asprintf("timeout:%d\n", timeout);
                            timeoutTimer->setInterval(timeout * 1000);
                        } else if (tmp == "blksize") {
                            pos += (int) strlen("blksize") + 1;
                            tmp = "";
                            tmp = &ptr[pos];
                            blockSize = tmp.toUInt(&ok, 10);
                            if (!ok) {
                                sendErrorCode(EUNDEF);
                                if (file)
                                    file->close();
                                clearStatus();
                                return;
                            }
                            char tt[256] = {0};
                            ptr += sprintf(tt, "%d", blockSize) + 1;
                            //print << __LINE__ << blockSize;
                        } else {
                            sendErrorCode(EUNDEF);
                            if (file)
                                file->close();
                            clearStatus();
                            return;
                        }
                    }
                    if (timeoutTimer) {
                        timeoutTimer->start();
                    }

                    cmd = qToBigEndian((uint16_t) _OACK);
                    memset(sendBuf, 0, BUFFER_SIZE);
                    sptr = sendBuf;
                    memcpy(sptr, &cmd, 2);
                    sptr += 2;
                    memcpy(sptr, "tsize", strlen("tsize"));
                    sptr += strlen("tsize") + 1;
                    if (code == _RRQ) {
                        file_size = getFileSize(curFilePath + pre_filePath);
                        //print << __LINE__ << file_size;
                        totalSize = file_size;
                    } else {
                    }
                    sptr += sprintf(sptr, "%d", file_size);
                    sptr++;
                    if (timeout) {
                        memcpy(sptr, "timeout", strlen("timeout"));
                        sptr += strlen("timeout") + 1;
                        sptr += sprintf(sptr, "%d", timeout);
                        sptr++;
                        //print << QString::asprintf("111timeout:%d\n", timeout);
                    }
                    if (blockSize) {
                        memcpy(sptr, "blksize", strlen("blksize"));
                        sptr += strlen("blksize") + 1;
                        sptr += sprintf(sptr, "%d", blockSize);
                        sptr++;
                        //print << __LINE__ << blockSize;
                    } else {
                        blockSize = BLOCK_SIZE;
                        //print << __LINE__ << blockSize;
                    }
                    {
                        QByteArray data = QByteArray(sendBuf, (uint32_t) (sptr - sendBuf));
                        //print << __LINE__ << data;

                        udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
                    }
                    break; // read request
                case _DATA:
                    //print << "_DATA\n";
                    if (!file) {
                        timeoutTimer->stop();
                        sendErrorCode(ENOTFOUND);
                        clearStatus();
                        break;
                    }
                    readbytes = readBuf.count() - 4;
                    pos += 2;

                    memset(sendBuf, 0, BUFFER_SIZE);
                    sptr = sendBuf;
                    printProcess(totalSize, totalSize - file_size);
                    code = qToBigEndian((uint16_t) _ACK);
                    memcpy(sptr, &code, 2);
                    sptr += 2;
                    memcpy(&block_current_index, &ptr[pos - 2], 2);
                    memcpy(sptr, &block_current_index, 2);

                    sptr += 2;
                    if (qToBigEndian(block_current_index) == blockIndex) {
                        //print << __LINE__ << qToBigEndian(block_current_index) << blockIndex;

                        blockIndex++;
                        if (readbytes >= blockSize) {
                            readLen = file->write(&ptr[pos], blockSize);
                            if (readLen < 0) {
                                sendErrorCode(ENOTFOUND);
                                clearStatus();
                                break;
                            }
                            file_size -= readLen;

                            //print << "writing" << __LINE__ << file_size << readLen << readbytes;
                        } else {
                            readLen = file->write(&ptr[pos], readbytes);
                            if (readLen < 0) {
                                sendErrorCode(ENOTFOUND);
                                clearStatus();
                                break;
                            }
                            file_size -= readLen;
                            //print << "write finished" << __LINE__ << file_size << readLen;
                            // if(file_size<=0)
                            {
                                file->close();
                                timeoutTimer->stop();
                                endtftp = 1;
                            }
                        }
                    }

                    {
                        QByteArray data = QByteArray(sendBuf, (uint32_t) (sptr - sendBuf));
                        //print << __LINE__;
                        //print << __LINE__ << destAddress << destPort;
                        udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
                    }
                    if (endtftp) {
                        timeoutTimer->stop();

                        printProcess(totalSize, totalSize);
                        //print << "\ntftp done\n";
                        clearStatus();
                    }
                    break; // data packet
                case _ACK:
                    if (endtftp) {
                        timeoutTimer->stop();

                        printProcess(totalSize, totalSize);
                        //print << "\ntftp done\n";
                        clearStatus();
                        break;
                    }
                    if (!file) {
                        sendErrorCode(ENOTFOUND);
                        break;
                    }
                    memset(sendBuf, 0, BUFFER_SIZE);
                    sptr = sendBuf;
                    if (file_size < blockSize) {
                        readLen = file->read(sptr + 4, file_size);
                        readLen = file_size;
                        endtftp = 1;
                        file->close();
                    } else {
                        readLen = file->read(sptr + 4, blockSize);
                        //readLen = blockSize*readLen;
                    }
                    //timerout->start();
                    file_size -= readLen;
                    //print << __LINE__ << _DATA << "blocksize" << blockSize << readLen;
                    code = qToBigEndian((uint16_t) _DATA);
                    //print << __LINE__ << code;
                    memcpy(sptr, &code, 2);
                    sptr += 2;
                    index = qToBigEndian((uint16_t) blockIndex);
                    //print << __LINE__ << index;
                    memcpy(sptr, &index, 2);
                    sptr += 2;
                    blockIndex++;
                    sptr += readLen;
                    {
                        QByteArray data = QByteArray(sendBuf, (uint32_t) (sptr - sendBuf));
                        //print << __LINE__ << (uint32_t) (sptr - sendBuf);
                        //print << __LINE__ << destAddress << destPort;
                        udpSocket->writeDatagram(data.data(), data.count(), destAddress, destPort);
                    }
                    printProcess(totalSize, totalSize - file_size);

                    break; // acknowledgement
                case _ERROR:
                    timeoutTimer->stop();

                    //print << ("_ERROR\n");
                    clearStatus();
                    //print << "error!\r\n";
                    break; // error code
                case _OACK:
                    // if(timer)
                    // 	htimer_del(timer);
                    //print << ("_OACK\n");
                    break;
                default:
                    // if(timer)
                    // 	htimer_del(timer);
                    //print << ("default\n");
                    break;
                }
            }
        }
    }
}

// 客户端功能实现
void Tftp::setClientMode(bool clientMode)
{
    isClientMode = clientMode;
    if (isClientMode) {
        // 在客户端模式下，我们不绑定到特定端口，让系统自动分配
        if (udpSocket->state() == QAbstractSocket::BoundState) {
            udpSocket->close();
        }
        initFlag = true; // 客户端模式下不需要绑定初始化
    }
}

void Tftp::setDestinationAddress(const QHostAddress &address, quint16 port)
{
    destAddress = address;
    destPort = port;
}

void Tftp::startDownload(const QString &remoteFileName, const QString &localFileName)
{
    if (!isClientMode) {
        emit tftpErrorSignal("Must be in client mode to start download");
        return;
    }

    clearStatus();
    isClientMode = true;

    // 设置本地文件名
    QString localPath = localFileName.isEmpty() ? remoteFileName : localFileName;
    pre_filePath = localPath;

    // 打开本地文件用于写入
    file = new QFile(curFilePath + localPath);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        emit tftpErrorSignal(
            QString("Failed to open local file for writing: %1").arg(curFilePath + localPath));
        delete file;
        file = nullptr;
        return;
    }

    // 发送RRQ请求
    sendRRQ(remoteFileName);
}

void Tftp::startUpload(const QString &localFileName, const QString &remoteFileName)
{
    if (!isClientMode) {
        emit tftpErrorSignal("Must be in client mode to start upload");
        return;
    }

    clearStatus();
    isClientMode = true;

    // 检查本地文件是否存在
    QFileInfo fileInfo(curFilePath + localFileName);
    if (!fileInfo.exists() || fileInfo.isDir()) {
        emit tftpErrorSignal(QString("Local file not found: %1").arg(curFilePath + localFileName));
        return;
    }

    // 设置文件名
    QString remotePath = remoteFileName.isEmpty() ? localFileName : remoteFileName;
    pre_filePath = remotePath;

    // 打开本地文件用于读取
    file = new QFile(curFilePath + localFileName);
    if (!file->open(QIODevice::ReadOnly)) {
        emit tftpErrorSignal(
            QString("Failed to open local file for reading: %1").arg(curFilePath + localFileName));
        delete file;
        file = nullptr;
        return;
    }

    // 获取文件大小
    file_size = file->size();
    totalSize = file_size;

    // 发送WRQ请求
    sendWRQ(remotePath);
}

void Tftp::sendRRQ(const QString &fileName)
{
    uint16_t cmd = qToBigEndian((uint16_t) _RRQ);
    memset(sendBuf, 0, BUFFER_SIZE);
    sptr = sendBuf;

    // 写入命令
    memcpy(sptr, &cmd, 2);
    sptr += 2;

    // 写入文件名
    memcpy(sptr, fileName.toLatin1().constData(), fileName.length());
    sptr += fileName.length() + 1;

    // 写入模式
    memcpy(sptr, "octet", 5);
    sptr += 6;

    // 写入tsize选项
    memcpy(sptr, "tsize", 5);
    sptr += 6;
    sptr += sprintf(sptr, "0");
    sptr++;

    // 写入blksize选项
    memcpy(sptr, "blksize", 7);
    sptr += 8;
    sptr += sprintf(sptr, "512");
    sptr++;

    // 发送数据包
    QByteArray data(sendBuf, (int) (sptr - sendBuf));
    udpSocket->writeDatagram(data, destAddress, destPort);

    // 启动超时定时器
    timeoutTimer->start();
}

void Tftp::sendWRQ(const QString &fileName)
{
    uint16_t cmd = qToBigEndian((uint16_t) _WRQ);
    memset(sendBuf, 0, BUFFER_SIZE);
    sptr = sendBuf;

    // 写入命令
    memcpy(sptr, &cmd, 2);
    sptr += 2;

    // 写入文件名
    memcpy(sptr, fileName.toLatin1().constData(), fileName.length());
    sptr += fileName.length() + 1;

    // 写入模式
    memcpy(sptr, "octet", 5);
    sptr += 6;

    // 写入tsize选项
    memcpy(sptr, "tsize", 5);
    sptr += 6;
    sptr += sprintf(sptr, "%d", file_size);
    sptr++;

    // 写入blksize选项
    memcpy(sptr, "blksize", 7);
    sptr += 8;
    sptr += sprintf(sptr, "512");
    sptr++;

    // 发送数据包
    QByteArray data(sendBuf, (int) (sptr - sendBuf));
    udpSocket->writeDatagram(data, destAddress, destPort);

    // 启动超时定时器
    timeoutTimer->start();
}

void Tftp::processClientDatagram()
{
    while (udpSocket->hasPendingDatagrams()) {
        datagram = udpSocket->receiveDatagram();
        if (!datagram.isValid()) {
            continue;
        }

        // 重置定时器和重试计数
        if (timeoutTimer) {
            timeoutTimer->stop();
        }
        retryCount = 3;

        // 更新目标地址和端口（用于后续通信）
        destAddress = datagram.senderAddress();
        destPort = datagram.senderPort();

        readBuf = datagram.data();
        datagram.clear();

        uint16_t code = 0;
        char *ptr = readBuf.data();
        memcpy(&code, ptr, 2);
        code = qFromBigEndian(code);

        int pos = 2;
        uint16_t block_current_index = 0;
        int readLen = 0;
        uint32_t readbytes = 0;

        switch (code) {
        case _OACK:
            // 处理OACK响应
            while (pos < readBuf.count()) {
                QString option = &ptr[pos];
                pos += option.length() + 1;

                if (option == "tsize") {
                    QString value = &ptr[pos];
                    file_size = value.toUInt();
                    totalSize = file_size;
                    pos += value.length() + 1;
                } else if (option == "blksize") {
                    QString value = &ptr[pos];
                    blockSize = value.toUInt();
                    if (blockSize == 0) {
                        blockSize = BLOCK_SIZE;
                    }
                    pos += value.length() + 1;
                } else if (option == "timeout") {
                    QString value = &ptr[pos];
                    timeout = value.toInt();
                    timeoutTimer->setInterval(timeout * 1000);
                    pos += value.length() + 1;
                } else {
                    // 未知选项，跳过
                    pos += strlen(&ptr[pos]) + 1;
                }
            }

            // 如果是上传，发送第一个数据包
            if (file && file->openMode() == QIODevice::ReadOnly) {
                // 发送第一个数据包
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;

                uint16_t dataCmd = qToBigEndian((uint16_t) _DATA);
                memcpy(sptr, &dataCmd, 2);
                sptr += 2;

                uint16_t blockNum = qToBigEndian((uint16_t) 1);
                memcpy(sptr, &blockNum, 2);
                sptr += 2;

                int bytesRead = file->read(sptr, blockSize);
                if (bytesRead < 0) {
                    emit tftpErrorSignal("Failed to read from local file");
                    clearStatus();
                    return;
                }

                sptr += bytesRead;
                file_size -= bytesRead;

                QByteArray data(sendBuf, (int) (sptr - sendBuf));
                udpSocket->writeDatagram(data, destAddress, destPort);

                blockIndex = 2;
                if (bytesRead < blockSize) {
                    endtftp = 1;
                }
            } else if (file && file->openMode() == QIODevice::WriteOnly) {
                // 下载模式，等待DATA数据包
                // 发送ACK 0确认OACK
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;

                uint16_t ackCmd = qToBigEndian((uint16_t) _ACK);
                memcpy(sptr, &ackCmd, 2);
                sptr += 2;

                uint16_t blockNum = qToBigEndian((uint16_t) 0);
                memcpy(sptr, &blockNum, 2);
                sptr += 2;

                QByteArray data(sendBuf, (int) (sptr - sendBuf));
                udpSocket->writeDatagram(data, destAddress, destPort);

                blockIndex = 1;
            }
            break;

        case _DATA:
            // 处理DATA数据包（下载）
            if (!file || file->openMode() != QIODevice::WriteOnly) {
                sendErrorCode(ENOTFOUND);
                clearStatus();
                break;
            }

            pos += 2; // 跳过块号
            readbytes = readBuf.count() - 4;

            // 读取块号
            memcpy(&block_current_index, &ptr[2], 2);
            block_current_index = qFromBigEndian(block_current_index);

            if (block_current_index == blockIndex) {
                // 写入数据到文件
                readLen = file->write(&ptr[pos], readbytes);
                if (readLen < 0) {
                    sendErrorCode(EACCESS);
                    clearStatus();
                    break;
                }

                file_size -= readLen;
                printProcess(totalSize, totalSize - file_size);

                // 发送ACK
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;

                uint16_t ackCmd = qToBigEndian((uint16_t) _ACK);
                memcpy(sptr, &ackCmd, 2);
                sptr += 2;

                memcpy(sptr, &ptr[2], 2); // 发送接收到的块号
                sptr += 2;

                QByteArray data(sendBuf, (int) (sptr - sendBuf));
                udpSocket->writeDatagram(data, destAddress, destPort);

                blockIndex++;

                // 检查是否完成
                if (readbytes < blockSize) {
                    file->close();
                    endtftp = 1;
                    printProcess(totalSize, totalSize);
                    emit transferCompletedSignal(pre_filePath);
                    clearStatus();
                }
            } else {
                // 接收到重复的块，重新发送ACK
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;

                uint16_t ackCmd = qToBigEndian((uint16_t) _ACK);
                memcpy(sptr, &ackCmd, 2);
                sptr += 2;

                uint16_t prevBlock = qToBigEndian((uint16_t) (blockIndex - 1));
                memcpy(sptr, &prevBlock, 2);
                sptr += 2;

                QByteArray data(sendBuf, (int) (sptr - sendBuf));
                udpSocket->writeDatagram(data, destAddress, destPort);
            }
            break;

        case _ACK:
            // 处理ACK数据包（上传）
            if (!file || file->openMode() != QIODevice::ReadOnly) {
                sendErrorCode(ENOTFOUND);
                clearStatus();
                break;
            }

            memcpy(&block_current_index, &ptr[2], 2);
            block_current_index = qFromBigEndian(block_current_index);

            if (block_current_index == blockIndex - 1) {
                // 发送下一个数据包
                memset(sendBuf, 0, BUFFER_SIZE);
                sptr = sendBuf;

                uint16_t dataCmd = qToBigEndian((uint16_t) _DATA);
                memcpy(sptr, &dataCmd, 2);
                sptr += 2;

                uint16_t blockNum = qToBigEndian((uint16_t) blockIndex);
                memcpy(sptr, &blockNum, 2);
                sptr += 2;

                int bytesRead = file->read(sptr, blockSize);
                if (bytesRead < 0) {
                    emit tftpErrorSignal("Failed to read from local file");
                    sendErrorCode(EACCESS);
                    clearStatus();
                    return;
                }

                sptr += bytesRead;
                file_size -= bytesRead;

                QByteArray data(sendBuf, (int) (sptr - sendBuf));
                udpSocket->writeDatagram(data, destAddress, destPort);

                printProcess(totalSize, totalSize - file_size);

                blockIndex++;

                if (bytesRead < blockSize) {
                    endtftp = 1;
                    file->close();
                    printProcess(totalSize, totalSize);
                    emit transferCompletedSignal(pre_filePath);
                    clearStatus();
                }
            }
            break;

            // case _ERROR:
            //     // 处理错误响应
            //     pos += 2; // 跳过错误码
            //     QString errorMsg = &ptr[pos];
            //     emit tftpErrorSignal(QString("TFTP Error: %1").arg(errorMsg));
            //     clearStatus();
            //     break;

        default:
            // 未知数据包类型
            sendErrorCode(EBADOP);
            break;
        }
    }
}

int Tftp::getFileSize(QString path)
{
    QFileInfo info(path);
    if (info.exists())
        return info.size();
    return 0;
}

void Tftp::sendErrorCode(enum TftpError code)
{
    const char *errorCodeMsg[] = {
        "Undefined error code",
        "File not found",
        "Access violation",
        "Disk full or allocation exceeded",
        "Illegal TFTP operation",
        "Unknown transfer ID",
        "File already exists",
        "No such user",
    };
    const char *msg = errorCodeMsg[code];
    // 使用QByteArray避免缓冲区溢出
    QByteArray buf;
    buf.append((char) 0x00);
    buf.append((char) 0x05);
    buf.append((char) 0x00);
    buf.append((char) code);
    buf.append(msg);
    buf.append((char) 0x00);

    {
        udpSocket->writeDatagram(buf, destAddress, destPort);
    }
    emit tftpErrorSignal(curFilePath + pre_filePath + "\n" + msg);
}

void Tftp::clearStatus()
{
    transportType = 0;
    file_size = 0;
    blockSize = 0;
    blockIndex = 1;
    if (file) {
        //print << fileMd5(*file);
        file->close();
        file->deleteLater();
    }
    file = nullptr;
    endtftp = 0;
    totalSize = 0;
    retryCount = 3;
    progress_bak = 0;
    //    if(udpSocket)
    //    {
    //        QObject::disconnect(udpSocket, &QUdpSocket::readyRead, this, &Tftp::readPendingDatagramsSlots);
    //        udpSocket->deleteLater();
    //        udpSocket=nullptr;
    //    }
    this->dataAvalivabe = false;
}

const QString Tftp::fileMd5(const QString &path)
{
    QFile sourceFile(path);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }

    return QString();
}
const QString Tftp::fileMd5(QFile &sourceFile)
{
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;
    if (sourceFile.isOpen()) {
        sourceFile.close();
    }
    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }

    return QString();
}