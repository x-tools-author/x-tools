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
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QEventLoop>
#include <QApplication>

#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerDeviceMask.hh"

SAKDebuggerDevice::SAKDebuggerDevice(QSettings *settings,
                                     const QString &settingsGroup,
                                     QObject *parent)
    :QThread(parent)
    ,mMask(Q_NULLPTR)
{
    mMask = new SAKDebuggerDeviceMask(settings, settingsGroup);
    mParametersCtx.maskCtx = mMask->parametersContext();
    connect(mMask, &SAKDebuggerDeviceMask::parametersChanged,
            this, [&](){
        mParametersCtxMutex.lock();
        mParametersCtx.maskCtx = mMask->parametersContext();
        mParametersCtxMutex.unlock();
    });
}

SAKDebuggerDevice::~SAKDebuggerDevice()
{

}

void SAKDebuggerDevice::writeBytes(QByteArray bytes)
{
    mBytesVectorMutex.lock();
    if (bytes.length()){
        mBytesVector.append(bytes);
    }else{
        mBytesVector.append(QByteArray("empty"));
    }
    mBytesVectorMutex.unlock();
}

void SAKDebuggerDevice::setupMenu(QMenu *menu)
{
    menu->addAction(tr("Mask"), this, [=](){
        if (mMask->isHidden()) {
            mMask->show();
        } else {
            mMask->activateWindow();
        }
    });
}

QByteArray SAKDebuggerDevice::takeBytes()
{
    QByteArray bytes;
    mBytesVectorMutex.lock();
    if (mBytesVector.count()){
        bytes = mBytesVector.takeFirst();
    }
    mBytesVectorMutex.unlock();

    return bytes;
}

void SAKDebuggerDevice::run()
{
    QTimer *writeTimer = new QTimer;
    writeTimer->setInterval(5);
    writeTimer->setSingleShot(true);
    if (initialize()) {
        connect(this, &SAKDebuggerDevice::readyRead,
                this, [=](SAKDeviceProtectedSignal){
            QByteArray ret = read();
            if (ret.length()) {
                ret = mask(ret, true);
                emit bytesRead(ret);
            }
        }, Qt::DirectConnection);

        connect(writeTimer, &QTimer::timeout, this, [=](){
            QByteArray bytes = takeBytes();
            if (bytes.length()) {
                bytes = mask(bytes, false);
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

QByteArray SAKDebuggerDevice::read()
{
    return QByteArray();
}

QByteArray SAKDebuggerDevice::mask(const QByteArray &plaintext, bool isRxData)
{
    auto doMask = [](const QByteArray &plaintext, quint8 mask)->QByteArray{
        QByteArray ciphertext;
        for (int i = 0; i < plaintext.length(); i++) {
            quint8 value = quint8(plaintext.at(i));
            value ^= mask;
            ciphertext.append(reinterpret_cast<char*>(&value), 1);
        }

        return ciphertext;
    };

    QByteArray ciphertext;
    auto ctx = parametersContext();
    quint8 mask = isRxData ? ctx.maskCtx.rx : ctx.maskCtx.tx;
    bool enable = isRxData ? ctx.maskCtx.enableRx : ctx.maskCtx.enableTx;
    if (enable) {
        ciphertext = doMask(plaintext, mask);
    } else {
        ciphertext = plaintext;
    }

    return ciphertext;
}

SAKDebuggerDevice::SAKStructDevicePatametersContext
SAKDebuggerDevice::parametersContext()
{
    mParametersCtxMutex.lock();
    auto parasCtx = mParametersCtx;
    mParametersCtxMutex.unlock();

    return parasCtx;
}
