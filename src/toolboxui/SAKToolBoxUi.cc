/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QLineEdit>
#include <QRegularExpression>

#include "SAKToolBoxUi.hh"
#include "SAKInterface.hpp"
#include "SAKUiInterface.hh"
#include "SAKToolFactory.hh"
#include "SAKSerialPortToolUi.hh"
#include "SAKCommunicationTool.hh"
#include "SAKToolBoxUiParameters.hh"

#include "ui_SAKToolBoxUi.h"

SAKToolBoxUi::SAKToolBoxUi(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKToolBoxUi)
{
    ui->setupUi(this);
    mToolBox = new SAKToolBox(this);
    mToolBoxUiParameters = new SAKToolBoxUiParameters(this);
    mToolBoxUiParameters->setModal(true);
    mToolBoxUiParameters->setupInputMasker(mToolBox->getInputMaskerTool());
    mToolBoxUiParameters->setupOutputMasker(mToolBox->getOutputMaskerTool());

    mCycleSendingTimer = new QTimer(this);
    connect(mCycleSendingTimer, &QTimer::timeout,
            this, &SAKToolBoxUi::try2send);

    init();
}

SAKToolBoxUi::~SAKToolBoxUi()
{
    delete ui;
}

QList<int> SAKToolBoxUi::supportedCommuniticationTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool;
//         << SAKToolFactory::UdpClientTool
//         << SAKToolFactory::UdpServerTool
//         << SAKToolFactory::TcpClientTool
//         << SAKToolFactory::TcpServerTool
//         << SAKToolFactory::WebSocketClientTool
//         << SAKToolFactory::WebSocketServerTool
//         << SAKToolFactory::BleCentral;
    return list;
}

void SAKToolBoxUi::setupCommuniticationTool(int type)
{
    if (mCommunicationTool) {
        return;
    }

    mToolBox->setupComunicationTool(type);
    setWindowTitle(communiticationToolName(type));

    auto var = mToolBox->property("communication");
    mCommunicationTool = var.value<SAKCommunicationTool*>();

    // Clear widget.
    auto l = ui->widgetCommunicationToolUi->layout();
    while (!l->isEmpty()) {
        auto item = l->itemAt(0);
        l->removeItem(item);
    }

    // Setup communication tool.
    mCommunicationToolUi = communiticationToolUi(type);
    if (!mCommunicationToolUi) {
        qCWarning(mLoggingCategory) << "mCommunicationToolUi is nullptr";
        return;
    }

    l->addWidget(mCommunicationToolUi);
    mCommunicationToolUi->setupCommunicationTool(mCommunicationTool);

    //auto outputAnalyzer = mToolBox->getOutputAnalyzerTool();
    connect(mToolBox, &SAKToolBox::isWorkingChanged,
            this, &SAKToolBoxUi::onIsWorkingChanged);
    connect(mCommunicationTool, &SAKCommunicationTool::bytesInputted,
            this, &SAKToolBoxUi::onTooBoxBytesInputted);
    connect(mCommunicationTool, &SAKCommunicationTool::bytesOutputted,
            this, &::SAKToolBoxUi::onTooBoxBytesOutputted);

    onIsWorkingChanged();
}

QString SAKToolBoxUi::communiticationToolName(int type)
{
    if (type == SAKToolFactory::SerialportTool) {
        return tr("SerialPort");
    } else if (type == SAKToolFactory::UdpClientTool) {
        return tr("UDP Client");
    } else if (type == SAKToolFactory::UdpServerTool) {
        return tr("UDP Server");
    } else if (type == SAKToolFactory::TcpClientTool) {
        return tr("TCP Client");
    } else if (type == SAKToolFactory::TcpServerTool) {
        return tr("TCP Server");
    } else if (type == SAKToolFactory::WebSocketClientTool) {
        return tr("WebSocket Client");
    } else if (type == SAKToolFactory::WebSocketServerTool) {
        return tr("WebSocket Server");
    } else if (type == SAKToolFactory::BleCentral) {
        return tr("BLE Central");
    } else {
        return "Unknow";
    }
}

SAKCommunicationToolUi *SAKToolBoxUi::communiticationToolUi(int type)
{
    SAKCommunicationToolUi *w = nullptr;
    if (type == SAKToolFactory::SerialportTool) {
        w = new SAKSerialPortToolUi();
    } else {
        qCWarning(mLoggingCategory) << "Unknow type of communication tool ui!";
    }

    return w;
}

void SAKToolBoxUi::try2send()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    QString input = ui->comboBoxInput->currentText();
    QByteArray bytes = SAKInterface::string2array(input, format);
    mToolBox->send(bytes);
}

QString SAKToolBoxUi::dateTimeContext()
{
    static QString dateFormat = QLocale::system().dateFormat();

    QString dateTimeFormat = "";
    bool d = ui->checkBoxOutputDate->isChecked();
    bool t = ui->checkBoxOutputTime->isChecked();
    bool m = ui->checkBoxOutputMs->isChecked();
    if (d && t && m) {
        dateTimeFormat = dateFormat + " hh:mm:ss.zzz";
    } else if (d && t) {
        dateTimeFormat = dateFormat + " hh:mm:ss";
    } else if (t && m) {
        dateTimeFormat = " hh:mm:ss.zzz";
    }  else if (d && m) {
        dateTimeFormat = dateFormat + " zzz";
    } else if (d) {
        dateTimeFormat = dateFormat;
    } else if (t) {
        dateTimeFormat = "hh:mm:ss";
    } else if (m) {
        dateTimeFormat = "zzz";
    }

    return QDateTime::currentDateTime().toString(dateTimeFormat);
}

void SAKToolBoxUi::output2ui(const QByteArray &bytes,
                             const QVariant &context,
                             bool isRx)
{
    Q_UNUSED(context);
    int format = ui->comboBoxOutputFormat->currentData().toInt();
    QString str = SAKInterface::array2String(bytes, format);
    QString dt = dateTimeContext();
    QString flag = isRx ? "Rx" : "Tx";
    QString color = isRx ? "red" : "blue";

    flag = QString("<font color=%1>%2</font>").arg(color, flag);
    dt = QString("<font color=silver>%1</font>").arg(dt);

    QString info = QString("[%1 %2]").arg(dt, flag);
    info = QString("<font color=silver>%1</font>").arg(info);

    ui->textBrowserOutput->append(info + " " + str);
}

void SAKToolBoxUi::onIsWorkingChanged()
{
    bool isWorking = mToolBox->isWorking();
    ui->pushButtonCommunicationOpen->setEnabled(true);
    mCommunicationToolUi->updateUiState(isWorking);
    ui->pushButtonInputSend->setEnabled(isWorking);
    ui->comboBoxInputIntervel->setEnabled(isWorking);

    if (isWorking) {
        ui->pushButtonCommunicationOpen->setText(tr("Close"));
    } else {
        ui->pushButtonCommunicationOpen->setText(tr("Open"));
    }

    if (!isWorking) {
        mCycleSendingTimer->stop();
    }
}

void SAKToolBoxUi::onTooBoxBytesInputted(const QByteArray &bytes,
                                         const QVariant &context)
{
    if (!ui->checkBoxOutputTx->isChecked()) {
        return;
    }

    output2ui(bytes, context, false);
}

void SAKToolBoxUi::onTooBoxBytesOutputted(const QByteArray &bytes,
                                          const QVariant &context)
{
    if (!ui->checkBoxOutputRx->isChecked()) {
        return;
    }

    output2ui(bytes, context, true);
}

void SAKToolBoxUi::init()
{
    initUi();
    initSettings();
    initSignals();
}

void SAKToolBoxUi::initUi()
{
    initUiCommunication();
    initUiInput();
    initUiOutput();
}

void SAKToolBoxUi::initUiCommunication()
{
    ui->widgetCommunicationToolUi->setLayout(new QHBoxLayout());
    ui->widgetCommunicationToolUi->layout()->setContentsMargins(0, 0, 0, 0);
}

void SAKToolBoxUi::initUiInput()
{
    ui->comboBoxInputIntervel->addItem(tr("Disable"), -1);
    for (int i = 10; i <= 100; i += 10) {
        ui->comboBoxInputIntervel->addItem(QString::number(i), i);
    }
    for (int i = 200; i <= 1000; i += 100) {
        ui->comboBoxInputIntervel->addItem(QString::number(i), i);
    }
    for (int i = 2000; i <= 5000; i += 1000) {
        ui->comboBoxInputIntervel->addItem(QString::number(i), i);
    }
}

void SAKToolBoxUi::initUiOutput()
{
    ui->checkBoxOutputRx->setChecked(true);
    ui->checkBoxOutputTx->setChecked(true);
    ui->checkBoxOutputWrap->setChecked(true);
    ui->checkBoxOutputTime->setChecked(true);
    ui->textBrowserOutput->document()->setMaximumBlockCount(2000);
}

void SAKToolBoxUi::initSettings()
{
    initSettingsCommunication();
    initSettingsInput();
    initSettingsOutput();
}

void SAKToolBoxUi::initSettingsCommunication()
{

}

void SAKToolBoxUi::initSettingsInput()
{
    onComboBoxInputFormatActivated();
}

void SAKToolBoxUi::initSettingsOutput()
{

}

void SAKToolBoxUi::initSignals()
{
    initSignalsCommunication();
    initSignalsInput();
    initSignalsOutput();
}

void SAKToolBoxUi::initSignalsCommunication()
{
    connect(ui->pushButtonCommunicationSettings, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonCommunicationSettingsClicked);
    connect(ui->pushButtonCommunicationOpen, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonCommunicationOpenClicked);
}

void SAKToolBoxUi::initSignalsInput()
{
    connect(ui->pushButtonInputSettings, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonInputSettingsClicked);
    connect(ui->comboBoxInputIntervel, &QComboBox::currentTextChanged,
            this, &SAKToolBoxUi::onComboBoxInputIntervelCurrentIndexChanged);
    connect(ui->pushButtonInputSend, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonInputSendClicked);
    connect(ui->comboBoxInputFormat, &QComboBox::activated,
            this, &SAKToolBoxUi::onComboBoxInputFormatActivated);
}

void SAKToolBoxUi::initSignalsOutput()
{
    connect(ui->pushButtonOutputSettings, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonOutputSettingsClicked);
    connect(ui->checkBoxOutputWrap, &QCheckBox::clicked,
            this, &SAKToolBoxUi::onCheckBoxOutputWrapClicked);
}

void SAKToolBoxUi::onPushButtonCommunicationSettingsClicked()
{
    mToolBoxUiParameters->showDialog(0);
}

void SAKToolBoxUi::onPushButtonCommunicationOpenClicked()
{
    ui->pushButtonCommunicationOpen->setEnabled(false);
    if (mToolBox->isWorking()) {
        mToolBox->close();
    } else {
        mToolBox->open();
    }
}

void SAKToolBoxUi::onPushButtonOutputSettingsClicked()
{
    mToolBoxUiParameters->showDialog(2);
}

void SAKToolBoxUi::onCheckBoxOutputWrapClicked()
{
    if (ui->checkBoxOutputWrap->isChecked()) {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::WrapAnywhere);
    } else {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::NoWrap);
    }
}

void SAKToolBoxUi::onPushButtonInputSettingsClicked()
{
    mToolBoxUiParameters->showDialog(1);
}

void SAKToolBoxUi::onPushButtonInputSendClicked()
{
    if (ui->comboBoxInput->currentText().isEmpty()) {
        qCInfo(mLoggingCategory) << "Input text is empty,"
                                    " the operation will be ignored!";
        QApplication::beep();
        ui->comboBoxInput->setFocus();
        return;
    }

    try2send();
}

void SAKToolBoxUi::onComboBoxInputIntervelCurrentIndexChanged()
{
    int interval = ui->comboBoxInputIntervel->currentText().toInt();
    interval = interval < 10 ? 10 : interval;
    qCInfo(mLoggingCategory) << "Start interval sending, the interval is:"
                             << interval;

    if (ui->comboBoxInputIntervel->currentIndex() == 0) {
        mCycleSendingTimer->stop();
    } else {
        mCycleSendingTimer->start(interval);
    }
}

void SAKToolBoxUi::onComboBoxInputFormatActivated()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    auto lineEdit = ui->comboBoxInput->lineEdit();
    SAKUiInterface::setValidator(lineEdit, format);
}
