/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef QTCRYPTOGRAPHICHASHCALCULATOR_HH
#define QTCRYPTOGRAPHICHASHCALCULATOR_HH

#include <QThread>

class SAKToolFileChecker;

class QtCryptographicHashCalculator:public QThread
{
    Q_OBJECT
public:
    QtCryptographicHashCalculator(SAKToolFileChecker *controller, QObject *parent = Q_NULLPTR);
private:
    SAKToolFileChecker *cryptographicHashController;
    // ------------------------------------------------------------------------
    void run() final;
signals:
    void outputMessage(QString msg, bool isErrMsg);
    void updateResult(QByteArray result);
    void progressBarMaxValueChanged(qint64 value);
    void updateProgressBar(qint64 currentValue);
    void remainTimeChanged(QString remainTime);
};

#endif
