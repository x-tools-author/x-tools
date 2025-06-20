﻿/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hashcalculator.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>

#include "filecheckassistant.h"

HashCalculator::HashCalculator(FileCheckAssistant* controller, QObject* parent)
    : QThread(parent)
    , m_cryptographicHashController(controller)
{
    connect(this, &HashCalculator::updateResult, controller, &FileCheckAssistant::updateResult);
    connect(this, &HashCalculator::outputMessage, controller, &FileCheckAssistant::outputMessage);
    connect(this,
            &HashCalculator::updateProgressBar,
            controller,
            &FileCheckAssistant::updateProgressBar);
    connect(this,
            &HashCalculator::remainTimeChanged,
            controller,
            &FileCheckAssistant::changeRemainTime);
}

void HashCalculator::run()
{
    QCryptographicHash::Algorithm algorithm = m_cryptographicHashController->algorithm();
    QCryptographicHash cryptographicHash(algorithm);
    cryptographicHash.reset();

    QString fileName = m_cryptographicHashController->fileName();
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        qint64 allBytes = file.size();
        emit progressBarMaxValueChanged(allBytes);

        qint64 consumeBytes = 0;
        qint64 remainBytes = 0;
        qint64 percent = 0;
        qint64 percenTemp = 0;
        qint64 startTime = 0;
        qint64 endTime = 0;
        qint64 consumeTime = 0;
        qint64 remainTime = 0;

        qint64 hours = 0;
        qint64 minutes = 0;
        qint64 seconds = 0;
        qint64 hoursTemp = 0;
        qint64 minutesTemp = 0;
        qint64 secondsTemp = 0;

        // The number of bytes read at a time
        const int dataBlock = 100 * 1024 * 1024;

        while (1) {
            startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

            QByteArray array = file.read(dataBlock);
            consumeBytes += array.length();
            remainBytes = allBytes - consumeBytes;

            // Effectively reduce the frequency of signal transmission
            percenTemp = (consumeBytes * 100) / allBytes;
            if (percenTemp != percent) {
                percent = percenTemp;
                emit updateProgressBar(percent);
            }

            // Returns an empty array. There are two possibilities.
            // One is that the file has been read, and the other is that the file has
            // been read incorrectly. Both cases are considered to be the end of the
            // check calculation
            if (array.isEmpty()) {
                emit outputMessage(tr("Calculating finished"), false);
                QApplication::beep();
                break;
            }

            cryptographicHash.addData(array);

            // Calculating remaining time
            endTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
            consumeTime = endTime - startTime;

            if (consumeTime != 0) {
                m_consumeTimeList.append(consumeTime);
                while (m_consumeTimeList.length() > 1000) {
                    m_consumeTimeList.removeFirst();
                }
                qint64 averageConsumeTime = 0;
                for (auto& var : m_consumeTimeList) {
                    averageConsumeTime += var;
                }
                averageConsumeTime = averageConsumeTime / m_consumeTimeList.count();
                if (averageConsumeTime > 0) {
                    remainTime = remainBytes / dataBlock * averageConsumeTime;

                    hoursTemp = (remainTime % (24 * 60 * 60 * 1000)) / (60 * 60 * 1000);
                    minutesTemp = ((remainTime % (24 * 60 * 60 * 1000)) % (60 * 60 * 1000))
                                  / (1 * 60 * 1000);
                    secondsTemp = (((remainTime % (24 * 60 * 60 * 1000)) % (60 * 60 * 1000))
                                   % (1 * 60 * 1000))
                                  / 1000;

                    if ((hours != hoursTemp) || (minutes != minutesTemp)
                        || (seconds != secondsTemp)) {
                        hours = hoursTemp;
                        minutes = minutesTemp;
                        seconds = secondsTemp;
                        emit remainTimeChanged(QString("%1:%2:%3")
                                                   .arg(QString::number(hours), 2, '0')
                                                   .arg(QString::number(minutes), 2, '0')
                                                   .arg(QString::number(seconds), 2, '0'));
                    }
                }
            }

            // Responsing the interruption requested
            if (isInterruptionRequested()) {
                return;
            }
        }

        QByteArray result = cryptographicHash.result();
        emit updateResult(result);
    } else {
        emit outputMessage(file.errorString(), true);
    }
}
