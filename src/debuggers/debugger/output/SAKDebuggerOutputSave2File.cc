/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QFile>
#include <QDialog>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>

#include "SAKDebuggerOutputSave2File.hh"
#include "ui_SAKDebuggerOutputSave2File.h"

SAKDebuggerOutputSave2File::SAKDebuggerOutputSave2File(QSettings
                                                       *settings,
                                                       QString settingGroup,
                                                       QWidget *parent)
    :QDialog(parent)
    ,m_settings(settings)
    ,ui(new Ui::SAKDebuggerOutputSave2File)
{
    // Initializing ui components
    ui->setupUi(this);
    setModal(true);

    // Initailizing ui component pointer
    m_pathLineEdit = ui->pathLineEdit;
    m_selectPushButton = ui->selectPushButton;
    m_readDataCheckBox = ui->readDataCheckBox;
    m_writtenDataCheckBox = ui->writtenDataCheckBox;
    m_timestampCheckBox = ui->timestampCheckBox;
    m_binRadioButton = ui->binRadioButton;
    m_utf8RadioButton = ui->utf8RadioButton;
    m_hexRadioButton = ui->hexRadioButton;
    m_okPushButton = ui->okPushButton;
    m_truncatePushButton = ui->truncatePushButton;

    connect(m_pathLineEdit, &QLineEdit::textChanged,
            this, &SAKDebuggerOutputSave2File::onPathLineEditTextChanged);
    connect(m_selectPushButton, &QPushButton::clicked,
            this, &SAKDebuggerOutputSave2File::onSelectPushButtonClicked);
    connect(m_readDataCheckBox, &QCheckBox::clicked,
            this, &SAKDebuggerOutputSave2File::onReadDataCheckBoxClicked);
    connect(m_writtenDataCheckBox, &QCheckBox::clicked,
            this, &SAKDebuggerOutputSave2File::onWrittenDataCheckBoxClicked);
    connect(m_timestampCheckBox, &QCheckBox::clicked,
            this, &SAKDebuggerOutputSave2File::onTimestampCheckBoxClicked);
    connect(m_binRadioButton, &QRadioButton::clicked,
            this, &SAKDebuggerOutputSave2File::onBinRadioButtonClicked);
    connect(m_hexRadioButton, &QRadioButton::clicked,
            this, &SAKDebuggerOutputSave2File::onHexRadioButtonClicked);
    connect(m_utf8RadioButton, &QPushButton::clicked,
            this, &SAKDebuggerOutputSave2File::onUtf8RadioButtonClicked);
    connect(m_truncatePushButton, &QPushButton::clicked,
            this, &SAKDebuggerOutputSave2File::onTruncatePushButtonClicked);

    // Initializing variables about settings
    QString groupString = settingGroup;
    m_settingsOutputPath = QString("%1/outputPath").arg(groupString);
    m_settingKeyReadData = QString("%1/readData").arg(groupString);
    m_settingKeyWrittenData = QString("%1/writtenData").arg(groupString);
    m_settingKeyTimestamp = QString("%1/saveTimestamp").arg(groupString);
    m_settingKeyDataType = QString("%1/dataType").arg(groupString);
    m_settings = settings;

    // ParametersContext will be signal parameter,
    // the step must be done, or will be error.
    qRegisterMetaType<ParametersContext>("SaveOutputDataParamters");

    // The task of thread is that writting data to file.
    m_saveOutputDataThread = new Save2FileThread(this);
    connect(this, &SAKDebuggerOutputSave2File::writeDataToFile,
            m_saveOutputDataThread, &Save2FileThread::writeDataToFile);
    m_saveOutputDataThread->start();

    // Readin setting info
    if (m_settings){
        QVariant var = m_settings->value(m_settingsOutputPath);
        if (!var.isNull()){
            m_pathLineEdit->setText(var.toString());
        }else{
            // Set default path for output file
            auto location = QStandardPaths::DesktopLocation;
            m_defaultPath = QStandardPaths::writableLocation(location);
            m_pathLineEdit->setText(m_defaultPath.append("/default.txt"));
        }

        struct TempInfo {
            QCheckBox *checkBox;
            QString key;
        };
        QList<TempInfo> list;
        list << TempInfo{m_readDataCheckBox, m_settingKeyReadData}
             << TempInfo{m_writtenDataCheckBox, m_settingKeyWrittenData}
             << TempInfo{m_timestampCheckBox, m_settingKeyTimestamp};

        for (int i = 0; i < list.length(); i++){
            TempInfo info = list.at(i);
            QVariant var = m_settings->value(info.key);
            if (var.isNull()){
                info.checkBox->setChecked(true);
            }else{
                info.checkBox->setChecked(var.toBool());
            }
        }


        var =  m_settings->value(m_settingKeyDataType);
        if (var.isNull()){
            m_hexRadioButton->setChecked(true);
        }else{
            int ret = var.toInt();
            switch (ret) {
            case ParametersContext::Bin:
                m_binRadioButton->setChecked(true);
                break;
            case ParametersContext::Hex:
                m_hexRadioButton->setChecked(true);
                break;
            case ParametersContext::Utf8:
                m_utf8RadioButton->setChecked(true);
                break;
            default:
                break;
            }
        }
    }
    setModal(true);
}

SAKDebuggerOutputSave2File::~SAKDebuggerOutputSave2File()
{
    delete ui;
    ui = Q_NULLPTR;
}

void SAKDebuggerOutputSave2File::bytesRead(QByteArray bytes)
{
    if (ui->checkBoxEnable->isChecked()) {
        auto parametersCtx = parameters(ParametersContext::Read);
        if (m_readDataCheckBox->isChecked()){
            emit writeDataToFile(bytes, parametersCtx);
        }
    }
}

void SAKDebuggerOutputSave2File::bytesWritten(QByteArray bytes)
{
    if (ui->checkBoxEnable->isChecked()) {
        auto parametersCtx = parameters(ParametersContext::Written);
        if (m_writtenDataCheckBox->isChecked()){
            emit writeDataToFile(bytes, parametersCtx);
        }
    }
}

SAKDebuggerOutputSave2File::ParametersContext
SAKDebuggerOutputSave2File::parameters(ParametersContext::DataType type)
{
    SAKDebuggerOutputSave2File::ParametersContext parametersCtx;
    parametersCtx.fileName = m_pathLineEdit->text().trimmed();
    if (m_binRadioButton->isChecked()){
        parametersCtx.format = ParametersContext::Bin;
    }else if (m_utf8RadioButton->isChecked()){
        parametersCtx.format = ParametersContext::Utf8;
    }else{
        parametersCtx.format = ParametersContext::Hex;
    }
    parametersCtx.type = type;
    parametersCtx.saveTimestamp = ui->timestampCheckBox->isChecked();
    return parametersCtx;
}

void SAKDebuggerOutputSave2File::onPathLineEditTextChanged(const QString &text)
{
    if (m_settings){
        m_settings->setValue(m_settingsOutputPath, text);
    }
}

void SAKDebuggerOutputSave2File::onSelectPushButtonClicked()
{
    QString datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString fileName;
    datetime.append(".txt");
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save to File"),
                                            QString("%1/%2").arg(m_defaultPath,
                                                                 datetime),
                                            QString("txt (*.txt)"));

    if (!fileName.isEmpty()){
        m_pathLineEdit->setText(fileName);
    }
}

void SAKDebuggerOutputSave2File::onTruncatePushButtonClicked()
{
    QString fileName = m_pathLineEdit->text();
    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadWrite | QFile::Truncate)){
        file.close();
    }
}

void SAKDebuggerOutputSave2File::onReadDataCheckBoxClicked()
{
    if (m_settings){
        m_settings->setValue(m_settingKeyReadData, m_readDataCheckBox->isChecked());
    }
}

void SAKDebuggerOutputSave2File::onWrittenDataCheckBoxClicked()
{
    if (m_settings){
        m_settings->setValue(m_settingKeyWrittenData,
                             m_writtenDataCheckBox->isChecked());
    }
}

void SAKDebuggerOutputSave2File::onTimestampCheckBoxClicked()
{
    if (m_settings){
        m_settings->setValue(m_settingKeyTimestamp,
                             m_timestampCheckBox->isChecked());
    }
}

void SAKDebuggerOutputSave2File::onBinRadioButtonClicked()
{
    if (m_settings){
        m_settings->setValue(m_settingKeyDataType, ParametersContext::Bin);
    }
}

void SAKDebuggerOutputSave2File::onHexRadioButtonClicked()
{
    if (m_settings){
        m_settings->setValue(m_settingKeyDataType, ParametersContext::Hex);
    }
}

void SAKDebuggerOutputSave2File::onUtf8RadioButtonClicked()
{
    if (m_settings){
        m_settings->setValue(m_settingKeyDataType, ParametersContext::Utf8);
    }
}

SAKDebuggerOutputSave2File::Save2FileThread::Save2FileThread(QObject *parent)
    :QThread (parent)
{

}

SAKDebuggerOutputSave2File::Save2FileThread::~Save2FileThread()
{
    // flush temp data
    m_threadWaitCondition.wakeAll();
    // exit thread
    requestInterruption();
    m_threadWaitCondition.wakeAll();
    wait();
}

void SAKDebuggerOutputSave2File::Save2FileThread::writeDataToFile(
        QByteArray data,
        SAKDebuggerOutputSave2File::ParametersContext parameters
        )
{
    if(parameters.fileName.isEmpty()){
        return;
    }

    DataInfoStruct dataInfo;
    dataInfo.data = data;
    dataInfo.parameters = parameters;
    m_dataListMutex.lock();
    m_dataList.append(dataInfo);
    m_dataListMutex.unlock();

    // Wake the thread to write data
    m_threadWaitCondition.wakeAll();
}

void SAKDebuggerOutputSave2File::Save2FileThread::run()
{
    QEventLoop eventLoop;
    while (true) {
        // wirte data to file
        while (true) {
            DataInfoStruct info = takeDataInfo();
            if (info.data.length()){
                innerWriteDataToFile(info.data, info.parameters);
            }else{
                break;
            }
        }

        // emmmm...
        eventLoop.processEvents();

        // if no interruption requested, the thread will sleep
        if (isInterruptionRequested()){
            break;
        }else{
            m_treadMutex.lock();
            m_threadWaitCondition.wait(&m_treadMutex);
            m_treadMutex.unlock();
        }
    }
}

void SAKDebuggerOutputSave2File::Save2FileThread::innerWriteDataToFile(
        QByteArray data,
        SAKDebuggerOutputSave2File::ParametersContext parameters)
{
    // If the size of file more than 1M, create a new file to save data
    if (QFile::exists(parameters.fileName)){
        QFileInfo fileInfo(parameters.fileName);
        QString fullPath = parameters.fileName;
        QString fileName = fullPath.split('/').last();
        QString path = fullPath.remove(fileName);
        if (fileInfo.size() > 1024*1024){
            QFile file(parameters.fileName);
            QString suffix = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            file.rename(QString("%1/backup_%2_%3").arg(path, suffix, fileName));
        }
    }

    QFile file(parameters.fileName);
    int format = parameters.format;
    QTextStream textStream(&file);

    QString dataString;
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
        dataString = bytes2String(data, format);
        QString dtStr = QDateTime::currentDateTime().toString("hh:mm:ss ");
                bool isRx = parameters.type == ParametersContext::Read;
        QString rxtx = isRx ? QString("Rx") : QString("Tx");
        QString outString = QString("[%1%2]%3")
                .arg(parameters.saveTimestamp ? dtStr : QString(""),
                     rxtx,
                     dataString);
        textStream << outString << "\n";
        file.close();
    }
}

SAKDebuggerOutputSave2File::Save2FileThread::DataInfoStruct
SAKDebuggerOutputSave2File::Save2FileThread::takeDataInfo()
{
    DataInfoStruct info;
    m_dataListMutex.lock();
    if (m_dataList.length()){
        info = m_dataList.takeFirst();
    }
    m_dataListMutex.unlock();

    return info;
}

QString SAKDebuggerOutputSave2File::Save2FileThread::bytes2String(QByteArray bytes,
                                                                  int format)
{
    QString str;
    switch (format) {
    case SAKDebuggerOutputSave2File::ParametersContext::Bin:
        for (int i = 0; i < bytes.length(); i++){
            QString temp = QString::number(int(bytes.at(i)), 2);
            str.append(temp + QString(" "));
        }
        str = str.trimmed();
        break;
    case SAKDebuggerOutputSave2File::ParametersContext::Hex:
        for (int i = 0; i < bytes.length(); i++){
            QString temp = QString("%1")
                    .arg(QString::number(int(bytes.at(i)), 16), 2, '0');
            str.append(temp + QString(" "));
        }
        str = str.trimmed();
        break;
    case SAKDebuggerOutputSave2File::ParametersContext::Utf8:
        str = QString::fromUtf8(bytes);
        break;
    default:
        str = tr("Unknown text format!");
        break;
    }

    return str;
}

