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
    :SAKToolBase (parent)
    ,ui (new Ui::SAKToolFileChecker)
    ,calculator (Q_NULLPTR)
{
    ui->setupUi(this);

    /*
     * 初始化ui指针
     */
    filePathlineEdit = ui->filePathlineEdit;
    algorithmComboBox = ui->algorithmComboBox;
    resultLineEdit = ui->resultLineEdit;
    calculatorProgressBar = ui->calculatorProgressBar;
    openPushButton = ui->openPushButton;
    startStopPushButton = ui->startStopPushButton;
    upperCheckBox = ui->upperCheckBox;
    messageLabel = ui->messageLabel;
    remainTimeLabel = ui->remainTimeLabel;

    /*
     * 设置加密散列算法
     */
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    QStringList algorithmsStringList;
    for (int i = 0; i < algorithms.keyCount(); i++){
        algorithmsStringList.append(QString(algorithms.key(i)));
    }
    algorithmComboBox->addItems(algorithmsStringList);
    algorithmComboBox->setCurrentText("Md5");

    /*
     * 初始化ui使能状态
     */
    filePathlineEdit->setReadOnly(true);
    resultLineEdit->setReadOnly(true);
//    startStopPushButton->setEnabled(false);
    calculatorProgressBar->setMinimum(0);
    calculatorProgressBar->setMaximum(100);
    calculatorProgressBar->setValue(0);

    /*
     * 5秒自动清除输出信息
     */
    clearMessageTimer.setInterval(5*1000);
    connect(&clearMessageTimer, &QTimer::timeout, this, &SAKToolFileChecker::clearMessage);

    upperCheckBox->setChecked(true);
    setWindowTitle(tr("文件校验工具"));
}

void SAKToolFileChecker::setUiEnable(bool enable)
{
    algorithmComboBox->setEnabled(enable);
    openPushButton->setEnabled(enable);
}

void SAKToolFileChecker::updateResult(QByteArray result)
{
    QString resultString = QString(result.toHex());
    if (upperCheckBox->isChecked()){
        resultLineEdit->setText(resultString.toUpper());
    }else{
        resultLineEdit->setText(resultString);
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

    messageLabel->setText(msg);
    clearMessageTimer.start();
}

void SAKToolFileChecker::updateProgressBar(int currentValue)
{
    calculatorProgressBar->setValue(currentValue);
}

void SAKToolFileChecker::changeRemainTime(QString remainTime)
{
    QString str = tr("估计剩余时间");
    remainTimeLabel->setText(QString("%1 %2").arg(str).arg(remainTime));
}

void SAKToolFileChecker::finished()
{
    on_startStopPushButton_clicked();
}

void SAKToolFileChecker::clearMessage()
{
    clearMessageTimer.stop();
    messageLabel->clear();
    remainTimeLabel->clear();
}

void SAKToolFileChecker::on_openPushButton_clicked()
{
    _fileName = QFileDialog::getOpenFileName();
    filePathlineEdit->setText(_fileName);
    if (!_fileName.isEmpty()){
        startStopPushButton->setEnabled(true);
    }

    calculatorProgressBar->setValue(0);
    resultLineEdit->clear();
    messageLabel->clear();
}

void SAKToolFileChecker::on_algorithmComboBox_currentIndexChanged(int index)
{
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    _algorithm = static_cast<QCryptographicHash::Algorithm>(algorithms.value(index));
    resultLineEdit->clear();
    calculatorProgressBar->setValue(0);
}

void SAKToolFileChecker::on_startStopPushButton_clicked()
{
    if (calculator){
        calculator->blockSignals(true);
        calculator->terminate();
        calculator->deleteLater();
        calculator = Q_NULLPTR;
        setUiEnable(true);
        startStopPushButton->setText(tr("开始计算"));
        setUiEnable(true);
    }else{
        calculator = new QtCryptographicHashCalculator(this);
        connect(calculator, &QThread::finished, this, &SAKToolFileChecker::finished);
        calculator->start();
        startStopPushButton->setText(tr("停止计算"));
        setUiEnable(false);
    }
}

void SAKToolFileChecker::on_upperCheckBox_clicked()
{
    QString temp = resultLineEdit->text();
    if (upperCheckBox->isChecked()){
        resultLineEdit->setText(temp.toUpper());
    }else{
        resultLineEdit->setText(temp.toLower());
    }
}
