/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKTestDevice.hh"
#include "SAKTestDebugPage.hh"
#include "SAKTestDeviceController.hh"

SAKTestDevice::SAKTestDevice(SAKTestDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,mDebugPage(debugPage)
{

}

SAKTestDevice::~SAKTestDevice()
{

}

void SAKTestDevice::run()
{
    QEventLoop eventLoop;
    SAKTestDeviceController *controller = mDebugPage->controller();
    SAKTestDeviceController::ParametersContext parameters = controller->parameters();
    qint64 oldReadTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qint64 oldWrittingTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (!parameters.openFailed){
        emit deviceStateChanged(true);
        while (true) {
            if(isInterruptionRequested()){
                break;
            }

            // Handling the thread events
            eventLoop.processEvents();

            // Count time
            qint64 newTimestamp =  QDateTime::currentDateTime().toMSecsSinceEpoch();
            qint64 deltaRead = newTimestamp - oldReadTimestamp;
            qint64 deltaWritten = newTimestamp - oldWrittingTimestamp;

            // Read data
            SAKTestDeviceController::ParametersContext parameters = controller->parameters();
            if (parameters.readCyclic){
                if (deltaRead > parameters.readInterval){
                    oldReadTimestamp = newTimestamp;
                    QByteArray bytes = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).toLatin1();
                    emit bytesRead(bytes);
                }
            }

            // Write data
            if (parameters.writeCyclic){
                if (deltaWritten > parameters.writtingInterval){
                    oldWrittingTimestamp = newTimestamp;
                    QByteArray bytes = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).toLatin1();
                    emit bytesWritten(bytes);
                }
            }

            // Write data manual
            while (true){
                QByteArray bytes = takeWaitingForWrittingBytes();
                if (bytes.length()){
                    emit bytesWritten(bytes);
                }else{
                    break;
                }
            }

            // Thread sleep
            if(isInterruptionRequested()){
                break;
            }else{
                mThreadMutex.lock();
                mThreadWaitCondition.wait(&mThreadMutex, SAK_DEVICE_THREAD_SLEEP_INTERVAL);
                mThreadMutex.unlock();
            }
        }
    }

    emit deviceStateChanged(false);
    emit messageChanged(parameters.errorString, false);
}
