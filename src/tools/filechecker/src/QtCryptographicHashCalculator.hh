/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
    SAKToolFileChecker *mCryptographicHashController;
    QList<qint64> consumeTimeList;
private:
    void run() final;
signals:
    void outputMessage(QString msg, bool isErrMsg);
    void updateResult(QByteArray result);
    void progressBarMaxValueChanged(qint64 value);
    void updateProgressBar(qint64 currentValue);
    void remainTimeChanged(QString remainTime);
};

#endif
