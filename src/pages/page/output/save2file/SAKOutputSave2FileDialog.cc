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

#include "SAKDebugPage.hh"
#include "SAKOutputSave2FileThread.hh"
#include "SAKOutputSave2FileDialog.hh"
#include "ui_SAKOutputSave2FileDialog.h"

SAKOutputSave2FileDialog::SAKOutputSave2FileDialog(SAKDebugPage *debugPage, QWidget *parent)
    :QDialog(parent)
    ,mSettings(Q_NULLPTR)
    ,mDebugPage(debugPage)
    ,mUi(new Ui::SAKOutputSave2FileDialog)
{
    // Initializing ui components
    mUi->setupUi(this);
    setModal(true);

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

    // Initializing variables about settings
    QString groupString = mDebugPage->settingsGroup();
    mSettingsOutputPath = QString("%1/outputPath").arg(groupString);
    mSettingKeyReadData = QString("%1/readData").arg(groupString);
    mSettingKeyWrittenData = QString("%1/writtenData").arg(groupString);
    mSettingKeyTimestamp = QString("%1/saveTimestamp").arg(groupString);
    mSettingKeyDataType = QString("%1/dataType").arg(groupString);
    mSettings = mDebugPage->settings();

    // ParametersContext will be signal parameter, the step must be done, or will be error.
    qRegisterMetaType<ParametersContext>("SaveOutputDataParamters");

    // The task of thread is that writting data to file.
    mSaveOutputDataThread = new SAKOutputSave2FileThread(this);
    connect(this, &SAKOutputSave2FileDialog::writeDataToFile, mSaveOutputDataThread, &SAKOutputSave2FileThread::writeDataToFile);
    mSaveOutputDataThread->start();

    // Readin setting info
    if (mSettings){
        QVariant var = mSettings->value(mSettingsOutputPath);
        if (!var.isNull()){
            mPathLineEdit->setText(var.toString());
        }else{
            // Set default path for output file
            mDefaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            mPathLineEdit->setText(mDefaultPath.append("/default.txt"));
        }

        struct TempInfo {
            QCheckBox *checkBox;
            QString key;
        };
        QList<TempInfo> list;
        list << TempInfo{mReadDataCheckBox, mSettingKeyReadData}
             << TempInfo{mWrittenDataCheckBox, mSettingKeyWrittenData}
             << TempInfo{mTimestampCheckBox, mSettingKeyTimestamp};

        for (auto info : list){
            QVariant var = mSettings->value(info.key);
            if (var.isNull()){
                info.checkBox->setChecked(true);
            }else{
                info.checkBox->setChecked(var.toBool());
            }
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

void SAKOutputSave2FileDialog::on_pathLineEdit_textChanged(const QString &text)
{
    if (mSettings){
        mSettings->setValue(mSettingsOutputPath, text);
    }
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
