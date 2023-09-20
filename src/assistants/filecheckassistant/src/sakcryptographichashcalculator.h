/*******************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#ifndef SAKCRYPTOGRAPHICHASHCALCULATOR_H
#define SAKCRYPTOGRAPHICHASHCALCULATOR_H

#include <QThread>

class SAKFileCheckAssistant;
class SAKCryptographicHashCalculator : public QThread {
  Q_OBJECT
 public:
        SAKCryptographicHashCalculator(SAKFileCheckAssistant* controller,
                                 QObject* parent = Q_NULLPTR);

 private:
        SAKFileCheckAssistant* mCryptographicHashController;
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
