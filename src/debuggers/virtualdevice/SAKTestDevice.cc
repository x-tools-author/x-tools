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
    :SAKDebugPageDevice(debugPage, parent)
    ,mDebugPage(debugPage)
{

}

SAKTestDevice::~SAKTestDevice()
{
#ifdef QT_DEBUG
    qDebug() << __FUNCTION__ << __LINE__;
#endif
}

bool SAKTestDevice::initializing(QString &errorString)
{
    errorString = QString("Unknown error");
    mController = qobject_cast<SAKTestDeviceController*>(mDebugPage->deviceController());
    mOldReadTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    mOldWrittingTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    return true;
}

bool SAKTestDevice::open(QString &errorString)
{
    SAKTestDeviceController::ParametersContext parameters = mController->parameters().value<SAKTestDeviceController::ParametersContext>();
    errorString = parameters.errorString.length() ? parameters.errorString : QString("Unknown error");
    return !parameters.openFailed;
}

QByteArray SAKTestDevice::read()
{
    QByteArray bytes;
    mNewReadTimestamp =  QDateTime::currentDateTime().toMSecsSinceEpoch();
    SAKTestDeviceController::ParametersContext parameters = mController->parameters().value<SAKTestDeviceController::ParametersContext>();
    if (parameters.readCyclic){
        qint64 deltaRead = mNewReadTimestamp - mOldReadTimestamp;
        if (deltaRead > parameters.readInterval){
            mOldReadTimestamp = mNewReadTimestamp;
            bytes = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).toLatin1();
        }
    }

    return bytes;
}

QByteArray SAKTestDevice::write(QByteArray bytes)
{
    return bytes;
}

QByteArray SAKTestDevice::writeForTest()
{
    QByteArray bytesWritten;
    SAKTestDeviceController::ParametersContext parameters = mController->parameters().value<SAKTestDeviceController::ParametersContext>();
    mNewWrittingTimestamp =  QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (parameters.writeCyclic){
        qint64 delta = mNewWrittingTimestamp - mOldWrittingTimestamp;
        if (delta > parameters.writtingInterval){
            mOldWrittingTimestamp = mNewWrittingTimestamp;
            bytesWritten = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).toLatin1();
        }
    }

    return bytesWritten;
}

void SAKTestDevice::close()
{
    // Nothing to do
}

void SAKTestDevice::free()
{
    // Nothing to do
}
