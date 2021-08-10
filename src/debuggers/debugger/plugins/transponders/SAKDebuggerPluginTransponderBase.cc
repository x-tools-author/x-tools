/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QTimer>
#include "SAKDebuggerPluginTransponderBase.hh"

SAKDebuggerPluginTransponderBase::SAKDebuggerPluginTransponderBase(QObject *parent)
    :QThread(parent)
{

}

SAKDebuggerPluginTransponderBase::~SAKDebuggerPluginTransponderBase()
{

}

void SAKDebuggerPluginTransponderBase::writeBytes(QByteArray bytes)
{
    mBytesVectorMutex.lock();
    if (bytes.length()){
        mBytesVector.append(bytes);
    }else{
        mBytesVector.append(QByteArray("empty"));
    }
    mBytesVectorMutex.unlock();
}

QByteArray SAKDebuggerPluginTransponderBase::takeBytes()
{
    QByteArray bytes;
    mBytesVectorMutex.lock();
    if (mBytesVector.count()){
        bytes = mBytesVector.takeFirst();
    }
    mBytesVectorMutex.unlock();

    return bytes;
}

void SAKDebuggerPluginTransponderBase::run()
{
    QTimer *writeTimer = new QTimer;
    writeTimer->setInterval(5);
    writeTimer->setSingleShot(true);
    if (initialize()) {
        connect(this, &SAKDebuggerPluginTransponderBase::readyRead,
                this, [=](SAKStructTransponderProtectedSignal){
            QByteArray ret = read();
            if (ret.length()) {
                emit bytesRead(ret);
            }
        }, Qt::DirectConnection);

        connect(writeTimer, &QTimer::timeout, this, [=](){
            QByteArray bytes = takeBytes();
            if (bytes.length()) {
                QByteArray ret = write(bytes);
                if (ret.length()) {
                    emit bytesWritten(ret);
                }
            }
            writeTimer->start();
        }, Qt::DirectConnection);

        writeTimer->start();
        exec();
    }

    writeTimer->stop();
    writeTimer->deleteLater();
    uninitialize();
}
