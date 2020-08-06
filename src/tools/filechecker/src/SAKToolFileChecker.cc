/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QMetaEnum>
#include <QFileDialog>

#include "QtCryptographicHashCalculator.hh"
#include "SAKToolFileChecker.hh"
#include "ui_SAKToolFileChecker.h"

SAKToolFileChecker::SAKToolFileChecker(QWidget *parent)
    :QWidget(parent)
    ,mUi (new Ui::SAKToolFileChecker)
    ,mCalculator (Q_NULLPTR)
{
    mUi->setupUi(this);

    /*
     * 初始化ui指针
     */
    mFilePathlineEdit = mUi->filePathlineEdit;
    mAlgorithmComboBox = mUi->algorithmComboBox;
    mResultLineEdit = mUi->resultLineEdit;
    mCalculatorProgressBar = mUi->calculatorProgressBar;
    mOpenPushButton = mUi->openPushButton;
    mStartStopPushButton = mUi->startStopPushButton;
    mUpperCheckBox = mUi->upperCheckBox;
    mMessageLabel = mUi->messageLabel;
    mRemainTimeLabel = mUi->remainTimeLabel;

    /*
     * 设置加密散列算法
     */
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    QStringList algorithmsStringList;
    for (int i = 0; i < algorithms.keyCount(); i++){
        algorithmsStringList.append(QString(algorithms.key(i)));
    }
    mAlgorithmComboBox->addItems(algorithmsStringList);
    mAlgorithmComboBox->setCurrentText("Md5");

    /*
     * 初始化ui使能状态
     */
    mFilePathlineEdit->setReadOnly(true);
    mResultLineEdit->setReadOnly(true);
//    startStopPushButton->setEnabled(false);
    mCalculatorProgressBar->setMinimum(0);
    mCalculatorProgressBar->setMaximum(100);
    mCalculatorProgressBar->setValue(0);

    /*
     * 5秒自动清除输出信息
     */
    mClearMessageTimer.setInterval(5*1000);
    connect(&mClearMessageTimer, &QTimer::timeout, this, &SAKToolFileChecker::clearMessage);

    mUpperCheckBox->setChecked(true);
    setWindowTitle(tr("文件校验工具"));
}

void SAKToolFileChecker::setUiEnable(bool enable)
{
    mAlgorithmComboBox->setEnabled(enable);
    mOpenPushButton->setEnabled(enable);
}

void SAKToolFileChecker::updateResult(QByteArray result)
{
    QString resultString = QString(result.toHex());
    if (mUpperCheckBox->isChecked()){
        mResultLineEdit->setText(resultString.toUpper());
    }else{
        mResultLineEdit->setText(resultString);
    }
}

void SAKToolFileChecker::outputMessage(QString msg, bool isErrMsg)
{
    if (isErrMsg){
        QApplication::beep();
        msg = QString("<font color=red>%1</font>").arg(msg);
    }else{
        msg = QString("<font color=blue>%1</font>").arg(msg);
    }

    mMessageLabel->setText(msg);
    mClearMessageTimer.start();
}

void SAKToolFileChecker::updateProgressBar(int currentValue)
{
    mCalculatorProgressBar->setValue(currentValue);
}

void SAKToolFileChecker::changeRemainTime(QString remainTime)
{
    QString str = tr("估计剩余时间");
    mRemainTimeLabel->setText(QString("%1 %2").arg(str).arg(remainTime));
}

void SAKToolFileChecker::finished()
{
    on_startStopPushButton_clicked();
}

void SAKToolFileChecker::clearMessage()
{
    mClearMessageTimer.stop();
    mMessageLabel->clear();
    mRemainTimeLabel->clear();
}

void SAKToolFileChecker::on_openPushButton_clicked()
{
    mFileName = QFileDialog::getOpenFileName();
    mFilePathlineEdit->setText(mFileName);
    if (!mFileName.isEmpty()){
        mStartStopPushButton->setEnabled(true);
    }

    mCalculatorProgressBar->setValue(0);
    mResultLineEdit->clear();
    mMessageLabel->clear();
}

void SAKToolFileChecker::on_algorithmComboBox_currentIndexChanged(int index)
{
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    mAlgorithm = static_cast<QCryptographicHash::Algorithm>(algorithms.value(index));
    mResultLineEdit->clear();
    mCalculatorProgressBar->setValue(0);
}

void SAKToolFileChecker::on_startStopPushButton_clicked()
{
    if (mCalculator){
        mCalculator->blockSignals(true);
        mCalculator->terminate();
        mCalculator->deleteLater();
        mCalculator = Q_NULLPTR;
        setUiEnable(true);
        mStartStopPushButton->setText(tr("开始计算"));
        setUiEnable(true);
    }else{
        mCalculator = new QtCryptographicHashCalculator(this);
        connect(mCalculator, &QThread::finished, this, &SAKToolFileChecker::finished);
        mCalculator->start();
        mStartStopPushButton->setText(tr("停止计算"));
        setUiEnable(false);
    }
}

void SAKToolFileChecker::on_upperCheckBox_clicked()
{
    QString temp = mResultLineEdit->text();
    if (mUpperCheckBox->isChecked()){
        mResultLineEdit->setText(temp.toUpper());
    }else{
        mResultLineEdit->setText(temp.toLower());
    }
}
