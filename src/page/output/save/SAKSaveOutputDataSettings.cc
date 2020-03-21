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
#include <QFile>
#include <QDialog>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>

#include "SAKSaveOutputDataThread.hh"
#include "SAKSaveOutputDataSettings.hh"
#include "ui_SAKSaveOutputDataSettings.h"

SAKSaveOutputDataSettings::SAKSaveOutputDataSettings(QWidget *parent)
    :QDialog (parent)
    ,ui (new Ui::SAKSaveOutputDataSettings)
{
    ui->setupUi(this);
    setModal(true);
    qRegisterMetaType<SaveOutputDataParamters>("SaveOutputDataParamters");

    pathLineEdit        = ui->pathLineEdit;
    setFilePushButton   = ui->setFilePushButton;
    binRadioButton      = ui->binRadioButton;
    utf8RadioButton     = ui->utf8RadioButton;
    hexRadioButton      = ui->hexRadioButton;
    closePushButton     = ui->closePushButton;
    clearFilePushButton = ui->clearFilePushButton;

    defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    pathLineEdit->setText(defaultPath.append("/default.txt"));

    saveOutputDataThread = new SAKSaveOutputDataThread;
    connect(this, &SAKSaveOutputDataSettings::writeDataToFile, saveOutputDataThread, &SAKSaveOutputDataThread::writeDataToFile);
    saveOutputDataThread->start();
}

SAKSaveOutputDataSettings::~SAKSaveOutputDataSettings()
{
    delete ui;
    saveOutputDataThread->terminate();
    delete saveOutputDataThread;
}

void SAKSaveOutputDataSettings::inputData(QByteArray data)
{
    parameters.fileName = pathLineEdit->text().trimmed();
    if (binRadioButton->isChecked()){
        parameters.format = SaveOutputDataParamters::Bin;
    }else if (utf8RadioButton->isChecked()){
        parameters.format = SaveOutputDataParamters::Utf8;
    }else{
        parameters.format = SaveOutputDataParamters::Hex;
    }

    emit writeDataToFile(data, parameters);
}

void SAKSaveOutputDataSettings::on_setFilePushButton_clicked()
{
    QString datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString fileName;
    if (binRadioButton->isChecked()){
        datetime.append(".bin");
        fileName = QFileDialog::getSaveFileName(this, tr("文件设置"), QString("%1/%2").arg(defaultPath).arg(datetime), QString("bin (*.bin)"));
    }else{
        datetime.append(".txt");
        fileName = QFileDialog::getSaveFileName(this, tr("文件设置"), QString("%1/%2").arg(defaultPath).arg(datetime), QString("txt (*.txt)"));
    }

    if (!fileName.isEmpty()){
        pathLineEdit->setText(fileName);
    }
}

void SAKSaveOutputDataSettings::on_clearFilePushButton_clicked()
{
    QString fileName = pathLineEdit->text();
    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadWrite | QFile::Truncate)){
        file.close();
    }
}
