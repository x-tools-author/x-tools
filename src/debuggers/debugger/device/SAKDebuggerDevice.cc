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
                                     QWidget *uiParent,
                                     QObject *parent)
    :QThread(parent)
    ,mMask(Q_NULLPTR)
{
    mMask = new SAKDebuggerDeviceMask(settings, settingsGroup, uiParent);
    mInnerParametersContext.maskCtx = mMask->parametersContext();
    connect(mMask, &SAKDebuggerDeviceMask::parametersChanged,
            this, [&](){
        mInnerParametersContextMutex.lock();
        mInnerParametersContext.maskCtx = mMask->parametersContext();
        mInnerParametersContextMutex.unlock();
    });

    mAnalyzer = new SAKDebuggerDeviceAnalyzer(settings, settingsGroup, uiParent);
    mInnerParametersContext.analyzerCtx = mAnalyzer->parametersContext();
    connect(mAnalyzer, &SAKDebuggerDeviceAnalyzer::parametersChanged,
            this, [&](){
        mInnerParametersContextMutex.lock();
        mInnerParametersContext.analyzerCtx = mAnalyzer->parametersContext();
        mInnerParametersContextMutex.unlock();
    });
    connect(mAnalyzer, &SAKDebuggerDeviceAnalyzer::clearTemp,
            this, [&](){
        mAnalyzerCtxMutex.lock();
        mAnalyzerCtx.bytesTemp.clear();
        mAnalyzerCtxMutex.unlock();
    });
}

SAKDebuggerDevice::~SAKDebuggerDevice()
{
#if 0
    mMask->deleteLater();
    mAnalyzer->deleteLater();
#endif
}

void SAKDebuggerDevice::writeBytes(QByteArray bytes)
{
    mBytesVectorMutex.lock();
    if (!isRunning()) {
        mBytesVectorMutex.unlock();
        return;
    }

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

    menu->addAction(tr("Analyzer"), this, [=](){
        if (mAnalyzer->isHidden()) {
            mAnalyzer->show();
        } else {
            mAnalyzer->activateWindow();
        }
    });
}

QVariant SAKDebuggerDevice::parametersContext()
{
    mParametersContextMutex.lock();
    QVariant ctx = mParametersContext;
    mParametersContextMutex.unlock();
    return ctx;
}

void SAKDebuggerDevice::setParametersContext(QVariant parametersContext)
{
    mParametersContextMutex.lock();
    mParametersContext = parametersContext;
    mParametersContextMutex.unlock();
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
            ReadContextVector contexts = read();
            for (const auto& context : contexts) {
                if (context.bytes.length()) {
                    auto ret = mask(context.bytes, true);
                    auto ctx = innerParametersContext();
                    if (ctx.analyzerCtx.enable) {
                        analyzer(ret);
                    } else {
                        emit bytesRead(ret, context.flag);
                    }
                }
            }
        }, Qt::DirectConnection);

        connect(writeTimer, &QTimer::timeout, this, [=](){
            QByteArray bytes = takeBytes();
            if (bytes.length()) {
                bytes = mask(bytes, false);
                WriteContext context = write(bytes);
                if (context.bytes.length()) {
                    emit bytesWritten(context.bytes, context.flag);
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
    auto ctx = innerParametersContext();
    quint8 mask = isRxData ? ctx.maskCtx.rx : ctx.maskCtx.tx;
    bool enable = isRxData ? ctx.maskCtx.enableRx : ctx.maskCtx.enableTx;
    if (enable) {
        ciphertext = doMask(plaintext, mask);
    } else {
        ciphertext = plaintext;
    }

    return ciphertext;
}

void SAKDebuggerDevice::analyzer(QByteArray data)
{
    // If the bytes of temp data is more than maxTempLength(2048) bytes,
    // a frame which length is maxTempLength will be emit.
    auto ctx = innerParametersContext();
    mAnalyzerCtxMutex.lock();
    mAnalyzerCtx.bytesTemp.append(data);
    if (mAnalyzerCtx.bytesTemp.length() >= mAnalyzerCtx.maxTempLangth) {
        QByteArray temp = mAnalyzerCtx.bytesTemp;
        mAnalyzerCtx.bytesTemp.clear();
        mAnalyzerCtxMutex.unlock();
        emit bytesRead(temp, "");
        return;
    }

    // The length of frame is fixed
    if (ctx.analyzerCtx.fixedLength) {
        if (ctx.analyzerCtx.length > 0) {
            while (mAnalyzerCtx.bytesTemp.length() >= ctx.analyzerCtx.length) {
                QByteArray temp = QByteArray(mAnalyzerCtx.bytesTemp.data(),
                                             ctx.analyzerCtx.length);
                mAnalyzerCtx.bytesTemp =
                        mAnalyzerCtx.bytesTemp.remove(0, ctx.analyzerCtx.length);
                emit bytesRead(temp, "");
            }
        } else {
            // If parameters is error(the length is less than 0 or equal to 0),
            // temp data will be clear!
            if (mAnalyzerCtx.bytesTemp.length()) {
                emit bytesRead(mAnalyzerCtx.bytesTemp, "");
                mAnalyzerCtx.bytesTemp.clear();
            }
        }

        mAnalyzerCtxMutex.unlock();
        return;
    }

    // Extract data according to the flags
    // If both of start-bytes and end-bytes are empty, temp data will be clear
    if (ctx.analyzerCtx.startFlags.isEmpty() && ctx.analyzerCtx.endFlags.isEmpty()){
        if (mAnalyzerCtx.bytesTemp.length()){
            QByteArray temp = mAnalyzerCtx.bytesTemp;
            mAnalyzerCtx.bytesTemp.clear();
            emit bytesRead(temp, "");
        }

        mAnalyzerCtxMutex.unlock();
        return;
    }


    while(1){
        // Ensure that bytes is enough
        if (mAnalyzerCtx.bytesTemp.length() < (ctx.analyzerCtx.startFlags.length() +
                                               ctx.analyzerCtx.endFlags.length())) {
            break;
        }

        // Match start-bytes
        bool startBytesMatched = true;
        if (ctx.analyzerCtx.startFlags.isEmpty()) {
            startBytesMatched = true;
        } else {
            int ret = mAnalyzerCtx.bytesTemp.indexOf(ctx.analyzerCtx.startFlags, 0);
            if (ret >= 0) {
                startBytesMatched = true;
                // Remove error data
                QByteArray temp = QByteArray(mAnalyzerCtx.bytesTemp.data(), ret);
                mAnalyzerCtx.bytesTemp.remove(0, ret);
                if (!temp.isEmpty()) {
                    emit bytesRead(temp, "");
                }
            } else {
                startBytesMatched = false;
            }
        }

        // Match end-bytes
        bool endBytesMatched = true;
        quint32 frameLength = 0;
        if (ctx.analyzerCtx.endFlags.isEmpty()) {
            endBytesMatched = true;
        } else {
            int ret = mAnalyzerCtx.bytesTemp.indexOf(
                        ctx.analyzerCtx.endFlags,
                        ctx.analyzerCtx.startFlags.length());
            if (ret >= 0) {
                endBytesMatched = true;
                frameLength = ret + ctx.analyzerCtx.endFlags.length();
            } else {
                endBytesMatched = false;
            }
        }

        // A completed data-frame has been extracted
        if (startBytesMatched && endBytesMatched) {
            QByteArray temp(mAnalyzerCtx.bytesTemp.data(), frameLength);
            mAnalyzerCtx.bytesTemp.remove(0, frameLength);
            if (!temp.isEmpty()) {
                emit bytesRead(temp, "");
            }
        }
    }
    mAnalyzerCtxMutex.unlock();
}

SAKDebuggerDevice::SAKStructDevicePatametersContext
SAKDebuggerDevice::innerParametersContext()
{
    mInnerParametersContextMutex.lock();
    auto parasCtx = mInnerParametersContext;
    mInnerParametersContextMutex.unlock();

    return parasCtx;
}
