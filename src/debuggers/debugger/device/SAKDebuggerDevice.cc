/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QEventLoop>
#include <QApplication>

#include "SAKDebuggerDevice.hh"
#include "SAKDebugPageDeviceMask.hh"

SAKDebuggerDevice::SAKDebuggerDevice(SAKDebugger *debugPage, QObject *parent)
    :QThread(parent)
    ,mDebugPage(debugPage)
{
    mDeviceMask = new SAKDebugPageDeviceMask(mDebugPage, Q_NULLPTR);
    mDeviceMask->setWindowModality(Qt::ApplicationModal);
    mSettingsPanelList << SettingsPanel{tr("Mask settings"),
                          qobject_cast<QWidget*>(mDeviceMask)};
}

SAKDebuggerDevice::~SAKDebuggerDevice()
{
    requestInterruption();
    mThreadWaitCondition.wakeAll();
    exit();
    wait();

    for (auto &var : mSettingsPanelList){
        var.panel->close();
        var.panel->deleteLater();
    }
}

void SAKDebuggerDevice::requestWakeup()
{
    mThreadWaitCondition.wakeAll();
}

void SAKDebuggerDevice::writeBytes(QByteArray bytes)
{
    mWaitingForWritingBytesListMutex.lock();
    if (bytes.length()){
        mWaitingForWritingBytesList.append(bytes);
    }else{
        mWaitingForWritingBytesList.append(QByteArray("empty"));
    }
    mWaitingForWritingBytesListMutex.unlock();
}

QList<SAKDebuggerDevice::SettingsPanel> SAKDebuggerDevice::settingsPanelList()
{
    return mSettingsPanelList;
}

QByteArray SAKDebuggerDevice::takeWaitingForWrittingBytes()
{
    QByteArray bytes;
    mWaitingForWritingBytesListMutex.lock();
    if (mWaitingForWritingBytesList.length()){
        bytes = mWaitingForWritingBytesList.takeFirst();
    }
    mWaitingForWritingBytesListMutex.unlock();

    return bytes;
}

void SAKDebuggerDevice::run()
{
    QEventLoop eventLoop;
    QString errorString;
    if (!initializing(errorString)){
        emit deviceStateChanged(false);
        emit messageChanged(errorString, false);
        free();
        return;
    }

    // Open the device
    if (open(errorString)){
        emit deviceStateChanged(true);
        while (true){
            if (isInterruptionRequested()){
                break;
            }

            // Read bytes from device
            QByteArray bytes = read();
            if (bytes.length() > 0){
                auto parasCtx = mDeviceMask->parametersContext();
                QByteArray temp;
                if (parasCtx.enableMask){
                    for (int i = 0; i < bytes.length(); i++){
                        quint8 value =  quint8(bytes.at(i));
                        value = bytes.at(i) ^ parasCtx.rxMask;
                        temp.append(reinterpret_cast<char*>(&value), 1);
                    }
                    bytes = temp;
                }
                emit bytesRead(bytes);
            }

            // Write bytes to device
            while (true) {
                bytes = takeWaitingForWrittingBytes();
                if (bytes.length() > 0){
                    bytes = write(bytes);
                    auto parasCtx = mDeviceMask->parametersContext();
                    QByteArray temp;
                    if (parasCtx.enableMask){
                        for (int i = 0; i < bytes.length(); i++){
                            quint8 value =  quint8(bytes.at(i));
                            value ^= parasCtx.txMask;
                            temp.append(reinterpret_cast<char*>(&value), 1);
                        }
                        bytes = temp;
                    }
                    emit bytesWritten(bytes);
                }else{
                    break;
                }
            }

            // Just for debugging data stream(for test page only)
            bytes = writeForTest();
            if (bytes.length()){
                auto parasCtx = mDeviceMask->parametersContext();
                QByteArray temp;
                if (parasCtx.enableMask){
                    for (int i = 0; i < bytes.length(); i++){
                        quint8 value =  quint8(bytes.at(i));
                        value ^= parasCtx.txMask;
                        temp.append(reinterpret_cast<char*>(&value));
                    }
                    bytes = temp;
                }
                emit bytesWritten(bytes);
            }

            // Chcked device state
            eventLoop.processEvents();
            if(!checkSomething(errorString)){
                emit messageChanged(errorString, false);
                break;
            }

            if(isInterruptionRequested()){
                break;
            }else{
                // Do something to make cpu happy
                mThreadMutex.lock();
                mThreadWaitCondition.wait(&mThreadMutex,
                                          SAK_DEVICE_THREAD_SLEEP_INTERVAL);
                mThreadMutex.unlock();
            }
        }

        close();
    }else{
        emit messageChanged(errorString, false);
    }

    emit deviceStateChanged(false);
    free();
}

bool SAKDebuggerDevice::initializing(QString &errorString)
{
    errorString = QString("Need to override");
    return false;
}

bool SAKDebuggerDevice::open(QString &errorString)
{
    errorString = QString("Need to override");
    return false;
}

QByteArray SAKDebuggerDevice::read()
{
    return QByteArray();
}

QByteArray SAKDebuggerDevice::write(QByteArray bytes)
{
    return bytes;
}

bool SAKDebuggerDevice::checkSomething(QString &errorString)
{
    errorString = QString("Unknown error");
    return true;
}

void SAKDebuggerDevice::close()
{
    // Nothing to do
}

void SAKDebuggerDevice::free()
{
    // Nothing to do
}

QByteArray SAKDebuggerDevice:: writeForTest()
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
    m_parametersCtxMutex.lock();
    auto parasCtx = m_parametersCtx;
    m_parametersCtxMutex.unlock();

    return parasCtx;
}
