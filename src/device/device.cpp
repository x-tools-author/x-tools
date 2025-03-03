/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "communication.h"

#include <QDebug>

Communication::Communication(QObject *parent)
    : AbstractIO(parent)
{
    connect(this, &Communication::started, this, [=]() {
        this->m_isWorking = true;
        emit this->isWorkingChanged();
    });
    connect(this, &Communication::finished, this, [=]() {
        this->m_isWorking = false;
        emit this->isWorkingChanged();
    });
    connect(this, &Communication::errorOccurred, this, [=](const QString &errorString) {
        qWarning() << "Error occured: " << errorString;
        exit();
        wait();
    });

    connect(this, &Communication::bytesRead, this, [=](const QByteArray &bytes, const QString &) {
        emit outputBytes(bytes);
    });
}

Communication::~Communication()
{
    if (isRunning()) {
        closeDevice();
    }
}

void Communication::openDevice()
{
    if (isRunning()) {
        closeDevice();
    }

    start();
}

void Communication::closeDevice()
{
    exit();
    wait();
}

void Communication::inputBytes(const QByteArray &bytes)
{
    if (bytes.isEmpty()) {
        qInfo() << "The input bytes is empty.";
        return;
    }

    emit invokeWriteBytes(bytes);
}

void Communication::run()
{
    QObject *obj = initDevice();
    if (!obj) {
        emit closed();
        return;
    }

    connect(this, &Communication::invokeWriteBytes, obj, [this](const QByteArray &bytes) {
        writeBytes(bytes);
    });

    emit opened();
    exec();

    deinitDevice();
    emit closed();
}
