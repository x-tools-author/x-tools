/********************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#include "sakfilecheckassistant.h"

#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>

#include "sakcryptographichashcalculator.h"
#include "ui_sakfilecheckassistant.h"

SAKFileCheckAssistant::SAKFileCheckAssistant(QWidget* parent)
    : QWidget(parent),
      mFileName(QString("C:/Windows/explorer.exe")),
      mAlgorithm(QCryptographicHash::Md5),
      mCalculator(Q_NULLPTR),
      mUi(new Ui::SAKFileCheckAssistant) {
  mUi->setupUi(this);

  // Initializing data member about ui
  mFilePathlineEdit = mUi->filePathlineEdit;
  mAlgorithmComboBox = mUi->algorithmComboBox;
  mResultLineEdit = mUi->resultLineEdit;
  mCalculatorProgressBar = mUi->calculatorProgressBar;
  mOpenPushButton = mUi->openPushButton;
  mStartStopPushButton = mUi->startStopPushButton;
  mUpperCheckBox = mUi->upperCheckBox;
  mMessageLabel = mUi->messageLabel;
  mRemainTimeLabel = mUi->remainTimeLabel;
  mFilePathlineEdit->setText(mFileName);

  // Appending algorithms to combo box
#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
  QMetaEnum algorithms =
      QMetaEnum::fromType<SAKToolFileCheckAssistant::Algorithm>();
#else
  QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
#endif
  QStringList algorithmsStringList;
  for (int i = 0; i < algorithms.keyCount(); i++) {
    algorithmsStringList.append(QString(algorithms.key(i)));
  }
  mAlgorithmComboBox->addItems(algorithmsStringList);
  mAlgorithmComboBox->setCurrentText("Md5");

  mFilePathlineEdit->setReadOnly(true);
  mResultLineEdit->setReadOnly(true);
  mCalculatorProgressBar->setMinimum(0);
  mCalculatorProgressBar->setMaximum(100);
  mCalculatorProgressBar->setValue(0);

  // It will clean the message which was showed on the info label when the timer
  // is timeout
  mClearMessageTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
  connect(&mClearMessageTimer, &QTimer::timeout, this,
          &SAKFileCheckAssistant::clearMessage);

  mUpperCheckBox->setChecked(true);
  setWindowTitle(tr("File Check Assistant"));
}

SAKFileCheckAssistant::~SAKFileCheckAssistant() {
  delete mUi;
  if (mCalculator) {
    mCalculator->blockSignals(true);
    mCalculator->requestInterruption();
    mCalculator->exit();
    mCalculator->wait();
    mCalculator->deleteLater();
    mCalculator = Q_NULLPTR;
  }
}

void SAKFileCheckAssistant::setUiEnable(bool enable) {
  mAlgorithmComboBox->setEnabled(enable);
  mOpenPushButton->setEnabled(enable);
}

QString SAKFileCheckAssistant::fileName() { return mFileName; }

QCryptographicHash::Algorithm SAKFileCheckAssistant::algorithm() {
  return mAlgorithm;
}

void SAKFileCheckAssistant::updateResult(QByteArray result) {
  QString resultString = QString(result.toHex());
  if (mUpperCheckBox->isChecked()) {
    mResultLineEdit->setText(resultString.toUpper());
  } else {
    mResultLineEdit->setText(resultString);
  }
}

void SAKFileCheckAssistant::outputMessage(QString msg, bool isErrMsg) {
  if (isErrMsg) {
    QApplication::beep();
    msg = QString("<font color=red>%1</font>").arg(msg);
  } else {
    msg = QString("<font color=blue>%1</font>").arg(msg);
  }

  mMessageLabel->setText(msg);
  mClearMessageTimer.start();
}

void SAKFileCheckAssistant::updateProgressBar(int currentValue) {
  mCalculatorProgressBar->setValue(currentValue);
}

void SAKFileCheckAssistant::changeRemainTime(QString remainTime) {
  QString str = tr("Remaining time");
  mRemainTimeLabel->setText(QString("%1 %2").arg(str, remainTime));
}

void SAKFileCheckAssistant::finished() { on_startStopPushButton_clicked(); }

void SAKFileCheckAssistant::clearMessage() {
  mClearMessageTimer.stop();
  mMessageLabel->clear();
  mRemainTimeLabel->clear();
}

void SAKFileCheckAssistant::on_openPushButton_clicked() {
  mFileName = QFileDialog::getOpenFileName();
  mFilePathlineEdit->setText(mFileName);
  if (!mFileName.isEmpty()) {
    mStartStopPushButton->setEnabled(true);
  }

  mCalculatorProgressBar->setValue(0);
  mResultLineEdit->clear();
  mMessageLabel->clear();
}

void SAKFileCheckAssistant::on_algorithmComboBox_currentIndexChanged(
    int index) {
#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
  QMetaEnum algorithms =
      QMetaEnum::fromType<SAKToolFileCheckAssistant::Algorithm>();
#else
  QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
#endif
  mAlgorithm =
      static_cast<QCryptographicHash::Algorithm>(algorithms.value(index));
  mResultLineEdit->clear();
  mCalculatorProgressBar->setValue(0);
}

void SAKFileCheckAssistant::on_startStopPushButton_clicked() {
  if (mCalculator) {
    mCalculator->blockSignals(true);
    mCalculator->requestInterruption();
    mCalculator->exit();
    mCalculator->wait();
    mCalculator->deleteLater();
    mCalculator = Q_NULLPTR;
    setUiEnable(true);
    mStartStopPushButton->setText(tr("Calculate"));
    setUiEnable(true);
  } else {
    mCalculator = new SAKCryptographicHashCalculator(this);
    connect(mCalculator, &QThread::finished, this,
            &SAKFileCheckAssistant::finished);
    mCalculator->start();
    mStartStopPushButton->setText(tr("StopCalculating"));
    setUiEnable(false);
  }
}

void SAKFileCheckAssistant::on_upperCheckBox_clicked() {
  QString temp = mResultLineEdit->text();
  if (mUpperCheckBox->isChecked()) {
    mResultLineEdit->setText(temp.toUpper());
  } else {
    mResultLineEdit->setText(temp.toLower());
  }
}
