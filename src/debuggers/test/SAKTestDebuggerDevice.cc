/****************************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKTestDebugger.hh"
#include "SAKTestDebuggerDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKTestDebuggerController.hh"

SAKTestDebuggerDevice::SAKTestDebuggerDevice(QSettings *settings,
                                             const QString &settingsGroup,
                                             QWidget *uiParent,
                                             QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mReadDataTimerId(-1)
    ,mWriteDateTimerId(-1)
{

}

SAKTestDebuggerDevice::~SAKTestDebuggerDevice()
{

}

void SAKTestDebuggerDevice::generateReadData(bool start, int interval)
{
    if (mReadDataTimerId != -1) {
        killTimer(mReadDataTimerId);
        mReadDataTimerId = -1;
    }

    if (start) {
        mReadDataTimerId = startTimer(interval > 50 ? interval : 50);
    }
}

void SAKTestDebuggerDevice::generateWriteData(bool start, int interval)
{
    if (mWriteDateTimerId != -1) {
        killTimer(mWriteDateTimerId);
        mWriteDateTimerId = -1;
    }

    if (start) {
        mWriteDateTimerId = startTimer(interval > 50 ? interval : 50);
    }
}

bool SAKTestDebuggerDevice::initialize()
{
    auto ctx = parametersContext()
            .value<SAKCommonDataStructure::SAKStructTestParametersContext>();
    bool openFailed = ctx.openFailed;
    QString msg = ctx.errorString;

    if (openFailed) {
        emit errorOccurred(msg);
    }

    return !openFailed;
}

SAKDebuggerDevice::ReadContextVector SAKTestDebuggerDevice::read()
{
    ReadContext ctx;
    ctx.bytes = QByteArray("Just for debuging");
    ctx.flag = QString("self");
    return ReadContextVector() << ctx;
}

SAKDebuggerDevice::WriteContext SAKTestDebuggerDevice::write(const QByteArray &bytes)
{
    Q_UNUSED(bytes);
    return WriteContext();
}

void SAKTestDebuggerDevice::uninitialize()
{
    // Nothing to do.
}

void SAKTestDebuggerDevice::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mReadDataTimerId) {
        emit readyRead(SAKDebuggerDevice::SAKDeviceProtectedSignal());
    } else if (event->timerId() == mWriteDateTimerId) {
        writeBytes(QByteArray("Bytes written!"));
    }

    SAKDebuggerDevice::timerEvent(event);
    return;
}
