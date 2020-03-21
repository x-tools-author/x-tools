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
#include <QDebug>
#include <QMetaEnum>
#include <QFileDialog>

#include "QtCryptographicHashCalculator.hh"
#include "QtCryptographicHashController.hh"
#include "ui_QtCryptographicHashController.h"

QtCryptographicHashController::QtCryptographicHashController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::QtCryptographicHashController)
    ,calculator (nullptr)
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
    connect(&clearMessageTimer, &QTimer::timeout, this, &QtCryptographicHashController::clearMessage);

    upperCheckBox->setChecked(true);
    setWindowTitle(tr("文件校验工具"));
}

void QtCryptographicHashController::setUiEnable(bool enable)
{
    algorithmComboBox->setEnabled(enable);
    openPushButton->setEnabled(enable);
}

void QtCryptographicHashController::updateResult(QByteArray result)
{
    QString resultString = QString(result.toHex());
    if (upperCheckBox->isChecked()){
        resultLineEdit->setText(resultString.toUpper());
    }else{
        resultLineEdit->setText(resultString);
    }
}

void QtCryptographicHashController::outputMessage(QString msg, bool isErrMsg)
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

void QtCryptographicHashController::updateProgressBar(int currentValue)
{
    calculatorProgressBar->setValue(currentValue);
}

void QtCryptographicHashController::changeRemainTime(QString remainTime)
{
    QString str = tr("估计剩余时间");
    remainTimeLabel->setText(QString("%1 %2").arg(str).arg(remainTime));
}

void QtCryptographicHashController::finished()
{
    on_startStopPushButton_clicked();
}

void QtCryptographicHashController::clearMessage()
{
    clearMessageTimer.stop();
    messageLabel->clear();
    remainTimeLabel->clear();
}

void QtCryptographicHashController::on_openPushButton_clicked()
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

void QtCryptographicHashController::on_algorithmComboBox_currentIndexChanged(int index)
{
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    _algorithm = static_cast<QCryptographicHash::Algorithm>(algorithms.value(index));
    resultLineEdit->clear();
    calculatorProgressBar->setValue(0);
}

void QtCryptographicHashController::on_startStopPushButton_clicked()
{
    if (calculator){
        calculator->blockSignals(true);
        calculator->terminate();
        calculator->deleteLater();
        calculator = nullptr;
        setUiEnable(true);
        startStopPushButton->setText(tr("开始计算"));
        setUiEnable(true);
    }else{
        calculator = new QtCryptographicHashCalculator(this);
        connect(calculator, &QThread::finished, this, &QtCryptographicHashController::finished);
        calculator->start();
        startStopPushButton->setText(tr("停止计算"));
        setUiEnable(false);
    }
}

void QtCryptographicHashController::on_upperCheckBox_clicked()
{
    QString temp = resultLineEdit->text();
    if (upperCheckBox->isChecked()){
        resultLineEdit->setText(temp.toUpper());
    }else{
        resultLineEdit->setText(temp.toLower());
    }
}
