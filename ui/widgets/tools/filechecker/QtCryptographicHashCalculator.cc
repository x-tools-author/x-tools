/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QApplication>

#include "QtCryptographicHashCalculator.hh"
#include "QtCryptographicHashController.hh"
QtCryptographicHashCalculator::QtCryptographicHashCalculator(QtCryptographicHashController *controller, QObject *parent)
    :QThread (parent)
    ,cryptographicHashController (controller)
{
    connect(this, &QtCryptographicHashCalculator::updateResult,
            controller, &QtCryptographicHashController::updateResult);
    connect(this, &QtCryptographicHashCalculator::outputMessage,
            controller, &QtCryptographicHashController::outputMessage);
    connect(this, &QtCryptographicHashCalculator::updateProgressBar,
            controller, &QtCryptographicHashController::updateProgressBar);
    connect(this, &QtCryptographicHashCalculator::remainTimeChanged,
            controller, &QtCryptographicHashController::changeRemainTime);
}

void QtCryptographicHashCalculator::run()
{
    QCryptographicHash::Algorithm algorithm = cryptographicHashController->algorithm();
    QCryptographicHash cryptographicHash(algorithm);
    cryptographicHash.reset();

    QString fileName = cryptographicHashController->fileName();
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)){
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

        // 每次读入的字节数
        int dataBlock = 1024*1024;

        while (1) {                       
            startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

            QByteArray array = file.read(dataBlock);
            consumeBytes += array.length();
            remainBytes = allBytes - consumeBytes;

            /*
             * 有效降低信号的发送频率
             */
            percenTemp = (consumeBytes*100)/allBytes;
            if (percenTemp != percent){
                percent = percenTemp;
                emit updateProgressBar(percent);
            }

            /*
             * 返回数组为空，有两种可能，一是文件读取完毕，另一个是文件读取出现错误。
             * 这两种情况都认为是校验计算结束
             */
            if (array.isEmpty()){
                outputMessage(tr("计算完成"), false);
                QApplication::beep();
                break;
            }

            cryptographicHash.addData(array);

            /*
             * 计算剩余时间
             */
            endTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
            consumeTime = endTime - startTime;
            if (consumeTime != 0){
                remainTime = remainBytes/dataBlock*consumeTime;

                hoursTemp = (remainTime%(24*60*60*1000))/(60*60*1000);
                minutesTemp = ((remainTime%(24*60*60*1000))%(60*60*1000))/(1*60*1000);
                secondsTemp = (((remainTime%(24*60*60*1000))%(60*60*1000))%(1*60*1000))/60;

                if ((hours != hoursTemp) || (minutes != minutesTemp) || (seconds != secondsTemp)){
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

        QByteArray result = cryptographicHash.result();
        emit updateResult(result);
    }else{
        outputMessage(file.errorString(), true);
    }
}
