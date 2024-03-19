/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QTimer>

#include "sakmaskertool.h"

SAKMaskerTool::SAKMaskerTool(QObject *parent)
    : SAKBaseTool{parent}
{
    m_enable = false;
}

SAKMaskerTool::~SAKMaskerTool()
{
    
}

void SAKMaskerTool::inputBytes(const QByteArray &bytes)
{
    if (!isEnable()) {
        emit outputBytes(bytes);
        return;
    }

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
            bytes = m_inputBytesList.takeFirst();
        }
        this->m_inputBytesListMutex.unlock();

        if (!bytes.isEmpty()) {
            QByteArray cookedBytes;
            for (int i = 0; i < bytes.length(); i++) {
                quint8 value = quint8(bytes.at(i));
                value ^= m_mask;
                cookedBytes.append(reinterpret_cast<char *>(&value), 1);
            }
            
            emit outputBytes(cookedBytes);
        }

        timer->start();
    });

    timer->start();
    exec();
    timer->stop();
    timer->deleteLater();
    timer = nullptr;
}
