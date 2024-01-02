/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QTimer>

#include "sakinterface.h"
#include "sakmaskertool.h"

SAKMaskerTool::SAKMaskerTool(QObject *parent)
    : SAKBaseTool{parent}
{
    mEnable = false;
}

SAKMaskerTool::~SAKMaskerTool()
{
    
}

void SAKMaskerTool::inputBytes(const QByteArray &bytes)
{
    m_inputBytesListMutex.lock();
    m_inputBytesList.append(bytes);
    m_inputBytesListMutex.unlock();
}

void SAKMaskerTool::setMaskCode(qint8 maskCode)
{
    m_mask = maskCode;
}

void SAKMaskerTool::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(5);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() {
        QByteArray bytes;
        this->m_inputBytesListMutex.lock();
        if (!this->m_inputBytesList.isEmpty()) {
            auto ctx = m_inputBytesList.takeFirst();
        }
        this->m_inputBytesListMutex.unlock();

        if (!bytes.isEmpty()) {
            QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
            QString hex = QString::fromLatin1(ba);
            outputMessage(QtInfoMsg, QString("%1<-%2").arg(mToolName, hex));

            if (this->enable()) {
                QByteArray cookedBytes;
                for (int i = 0; i < bytes.length(); i++) {
                    quint8 value = quint8(bytes.at(i));
                    value ^= m_mask;
                    cookedBytes.append(reinterpret_cast<char *>(&value), 1);
                }

                ba = SAKInterface::arrayToHex(cookedBytes, ' ');
                QString hex = QString::fromLatin1(ba);
                outputMessage(QtInfoMsg, QString("%1->%2").arg(mToolName, hex));
                emit bytesOutput(cookedBytes);
            } else {
                ba = SAKInterface::arrayToHex(bytes, ' ');
                QString hex = QString::fromLatin1(ba);
                outputMessage(QtInfoMsg, QString("%1->%2").arg(mToolName, hex));
                emit bytesOutput(bytes);
            }
        }

        timer->start();
    });

    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}
