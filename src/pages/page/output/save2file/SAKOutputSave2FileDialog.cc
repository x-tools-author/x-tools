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
#include <QFile>
#include <QDialog>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>

#include "SAKOutputSave2FileThread.hh"
#include "SAKOutputSave2FileDialog.hh"
#include "ui_SAKOutputSave2FileDialog.h"

SAKOutputSave2FileDialog::SAKOutputSave2FileDialog(QSettings *settings, QWidget *parent)
    :QDialog(parent)
    ,mSettings(settings)
    ,mSettingKeyReadData(QString("DebugPage/readData"))
    ,mSettingKeyWrittenData(QString("DebugPage/writtenData"))
    ,mSettingKeyTimestamp(QString("DebugPage/saveTimestamp"))
    ,mSettingKeyDataType(QString("DebugPage/dataType"))
    ,mUi(new Ui::SAKOutputSave2FileDialog)
{
    mUi->setupUi(this);
    setModal(true);
    // ParametersContext will be signal parameter, the step must be done, or will be error.
    qRegisterMetaType<ParametersContext>("SaveOutputDataParamters");

    // Initailizing ui component pointer
    mPathLineEdit = mUi->pathLineEdit;
    mSelectPushButton = mUi->selectPushButton;
    mReadDataCheckBox = mUi->readDataCheckBox;
    mWrittenDataCheckBox = mUi->writtenDataCheckBox;
    mTimestampCheckBox = mUi->timestampCheckBox;
    mBinRadioButton = mUi->binRadioButton;
    mUtf8RadioButton = mUi->utf8RadioButton;
    mHexRadioButton = mUi->hexRadioButton;
    mOkPushButton = mUi->okPushButton;
    mTruncatePushButton = mUi->truncatePushButton;

    // Set default path for output file
    mDefaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    mPathLineEdit->setText(mDefaultPath.append("/default.txt"));

    // The task of thread is that writting data to file.
    mSaveOutputDataThread = new SAKOutputSave2FileThread(this);
    connect(this, &SAKOutputSave2FileDialog::writeDataToFile, mSaveOutputDataThread, &SAKOutputSave2FileThread::writeDataToFile);
    mSaveOutputDataThread->start();

    // Readin setting info
    if (mSettings){
        QVariant var = mSettings->value(mSettingKeyReadData);
        if (var.isNull()){
            mReadDataCheckBox->setChecked(true);
        }else{
            mReadDataCheckBox->setChecked(var.toBool());
        }

        var = mSettings->value(mSettingKeyWrittenData);
        if (var.isNull()){
            mWrittenDataCheckBox->setChecked(true);
        }else{
            mWrittenDataCheckBox->setChecked(var.toBool());
        }

        var = mSettings->value(mSettingKeyTimestamp);
        if (var.isNull()){
            mTimestampCheckBox->setChecked(true);
        }else{
            mTimestampCheckBox->setChecked(var.toBool());
        }

        var =  mSettings->value(mSettingKeyDataType);
        if (var.isNull()){
            mHexRadioButton->setChecked(true);
        }else{
            int ret = var.toInt();
            switch (ret) {
            case ParametersContext::Bin:
                mBinRadioButton->setChecked(true);
                break;
            case ParametersContext::Hex:
                mHexRadioButton->setChecked(true);
                break;
            case ParametersContext::Utf8:
                mUtf8RadioButton->setChecked(true);
                break;
            default:
                break;
            }
        }
    }
}

SAKOutputSave2FileDialog::~SAKOutputSave2FileDialog()
{
    delete mUi;
    mUi = Q_NULLPTR;
}

void SAKOutputSave2FileDialog::bytesRead(QByteArray bytes)
{
    SAKOutputSave2FileDialog::ParametersContext parametersCtx = parameters(ParametersContext::Read);
    if (mReadDataCheckBox->isChecked()){
        emit writeDataToFile(bytes, parametersCtx);
    }
}

void SAKOutputSave2FileDialog::bytesWritten(QByteArray bytes)
{
    SAKOutputSave2FileDialog::ParametersContext parametersCtx = parameters(ParametersContext::Written);
    if (mWrittenDataCheckBox->isChecked()){
        emit writeDataToFile(bytes, parametersCtx);
    }
}

SAKOutputSave2FileDialog::ParametersContext SAKOutputSave2FileDialog::parameters(ParametersContext::DataType type)
{
    SAKOutputSave2FileDialog::ParametersContext parametersCtx;
    parametersCtx.fileName = mPathLineEdit->text().trimmed();
    if (mBinRadioButton->isChecked()){
        parametersCtx.format = ParametersContext::Bin;
    }else if (mUtf8RadioButton->isChecked()){
        parametersCtx.format = ParametersContext::Utf8;
    }else{
        parametersCtx.format = ParametersContext::Hex;
    }
    parametersCtx.type = type;
    parametersCtx.saveTimestamp = mUi->timestampCheckBox->isChecked();
    return parametersCtx;
}

void SAKOutputSave2FileDialog::on_selectPushButton_clicked()
{
    QString datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString fileName;
    datetime.append(".txt");
    fileName = QFileDialog::getSaveFileName(this, tr("Save file"), QString("%1/%2").arg(mDefaultPath).arg(datetime), QString("txt (*.txt)"));

    if (!fileName.isEmpty()){
        mPathLineEdit->setText(fileName);
    }
}

void SAKOutputSave2FileDialog::on_truncatePushButton_clicked()
{
    QString fileName = mPathLineEdit->text();
    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadWrite | QFile::Truncate)){
        file.close();
    }
}

void SAKOutputSave2FileDialog::on_readDataCheckBox_clicked()
{
    if (mSettings){
        mSettings->setValue(mSettingKeyReadData, mReadDataCheckBox->isChecked());
    }
}

void SAKOutputSave2FileDialog::on_writtenDataCheckBox_clicked()
{
    if (mSettings){
        mSettings->setValue(mSettingKeyWrittenData, mWrittenDataCheckBox->isChecked());
    }
}

void SAKOutputSave2FileDialog::on_timestampCheckBox_clicked()
{
    if (mSettings){
        mSettings->setValue(mSettingKeyTimestamp, mTimestampCheckBox->isChecked());
    }
}

void SAKOutputSave2FileDialog::on_binRadioButton_clicked()
{
    if (mSettings){
        mSettings->setValue(mSettingKeyDataType, ParametersContext::Bin);
    }
}

void SAKOutputSave2FileDialog::on_hexRadioButton_clicked()
{
    if (mSettings){
        mSettings->setValue(mSettingKeyDataType, ParametersContext::Hex);
    }
}

void SAKOutputSave2FileDialog::on_utf8RadioButton_clicked()
{
    if (mSettings){
        mSettings->setValue(mSettingKeyDataType, ParametersContext::Utf8);
    }
}
