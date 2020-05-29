/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef QTCRYPTOGRAPHICHASHCALCULATOR_HH
#define QTCRYPTOGRAPHICHASHCALCULATOR_HH

#include <QThread>

class QtCryptographicHashController;

class QtCryptographicHashCalculator:public QThread
{
    Q_OBJECT
public:
    QtCryptographicHashCalculator(QtCryptographicHashController *controller, QObject *parent = Q_NULLPTR);
private:
    QtCryptographicHashController *cryptographicHashController;
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
