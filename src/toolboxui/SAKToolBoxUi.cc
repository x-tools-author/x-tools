/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMetaEnum>
#include <QLineEdit>
#include <QRegularExpression>

#include "SAKSettings.hh"
#include "SAKToolBoxUi.hh"
#include "SAKInterface.hh"
#include "SAKUiInterface.hh"
#include "SAKToolFactory.hh"
#include "SAKCrcInterface.hh"
#include "SAKEmitterToolUi.hh"
#include "SAKDataStructure.hh"
#include "SAKResponserToolUi.hh"
#include "SAKPrestorerToolUi.hh"
#include "SAKBleCentralToolUi.hh"
#include "SAKSerialPortToolUi.hh"
#include "SAKCommunicationTool.hh"
#include "SAKSocketClientToolUi.hh"
#include "SAKSocketServerToolUi.hh"
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

    mCycleSendingTimer = new QTimer(this);
    connect(mCycleSendingTimer, &QTimer::timeout,
            this, &SAKToolBoxUi::try2send);
}

SAKToolBoxUi::~SAKToolBoxUi()
{
    delete ui;
}

QList<int> SAKToolBoxUi::supportedCommuniticationTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool
         << SAKToolFactory::UdpClientTool
         << SAKToolFactory::UdpServerTool
         << SAKToolFactory::TcpClientTool
         << SAKToolFactory::TcpServerTool
         << SAKToolFactory::WebSocketClientTool
         << SAKToolFactory::WebSocketServerTool
         << SAKToolFactory::BleCentralTool;
    return list;
}

void SAKToolBoxUi::initialize(int type)
{
    if (mCommunicationTool) {
        mCommunicationTool->deleteLater();
        mCommunicationTool = nullptr;
    }

    mToolBox->initialize(type);
    mCommunicationToolType = type;

    QString toolName = communiticationToolName(type);
    QIcon icon = communiticationToolIcon(type);
    setWindowTitle(toolName);
    setWindowIcon(icon);

    mCommunicationTool = mToolBox->getCommunicationTool();
    if (!mCommunicationTool) {
        qCWarning(mLoggingCategory) << "Initializing failed, "
                                       "tool box is invaliad!";
        return;
    }

    init();
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
    } else if (type == SAKToolFactory::BleCentralTool) {
        return tr("BLE Central");
    } else {
        return "Unknow";
    }
}

QIcon SAKToolBoxUi::communiticationToolIcon(int type)
{
    QString fileName;
    if (type == SAKToolFactory::SerialportTool) {
        fileName = ":/res/icon/IconSerialPort.svg";
    } else if (type == SAKToolFactory::UdpClientTool) {
        fileName = ":/res/icon/IconUdpClient.svg";
    } else if (type == SAKToolFactory::UdpServerTool) {
        fileName = ":/res/icon/IconUdpServer.svg";
    } else if (type == SAKToolFactory::TcpClientTool) {
        fileName = ":/res/icon/IconTcpClient.svg";
    } else if (type == SAKToolFactory::TcpServerTool) {
        fileName = ":/res/icon/IconTcpServer.svg";
    } else if (type == SAKToolFactory::WebSocketClientTool) {
        fileName = ":/res/icon/IconWebScoketClient.svg";
    } else if (type == SAKToolFactory::WebSocketServerTool) {
        fileName = ":/res/icon/IconWebSocketServer.svg";
    } else if (type == SAKToolFactory::BleCentralTool) {
        fileName = ":/res/icon/IconBlueTooth.svg";
    }

    QIcon icon(fileName);
    return icon;
}

SAKCommunicationToolUi *SAKToolBoxUi::communiticationToolUi(int type)
{
    SAKCommunicationToolUi *w = nullptr;
    if (type == SAKToolFactory::SerialportTool) {
        w = new SAKSerialPortToolUi();
    } else if (type == SAKToolFactory::UdpClientTool) {
        w = new SAKSocketClientToolUi();
    } else if (type == SAKToolFactory::UdpServerTool) {
        w = new SAKSocketServerToolUi();
    }  else if (type == SAKToolFactory::TcpClientTool) {
        w = new SAKSocketClientToolUi();
    } else if (type == SAKToolFactory::TcpServerTool) {
        w = new SAKSocketServerToolUi();
    }  else if (type == SAKToolFactory::WebSocketClientTool) {
        w = new SAKSocketClientToolUi();
    } else if (type == SAKToolFactory::WebSocketServerTool) {
        w = new SAKSocketServerToolUi();
    } else if (type == SAKToolFactory::BleCentralTool) {
        w = new SAKBleCentralToolUi();
    } else {
        qCWarning(mLoggingCategory) << "Unknow type of communication tool ui!";
    }

    return w;
}

void SAKToolBoxUi::try2send()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    QString input = ui->comboBoxInputText->currentText();
    auto ctx = mToolBoxUiParameters->parameterContext();

    int prefix = ctx.input.preprocessing.prefix;
    int suffix = ctx.input.preprocessing.suffix;
    int esc = ctx.input.preprocessing.escapeCharacter;

    QByteArray prefixData = SAKDataStructure::affixesData(prefix);
    QByteArray suffixData = SAKDataStructure::affixesData(suffix);

    input = SAKDataStructure::cookedString(esc, input);
    QByteArray bytes = SAKInterface::string2array(input, format);
    if (ctx.input.crc.enable) {
        int arithmetic = ctx.input.crc.arithmetic;
        int startIndex = ctx.input.crc.startIndex;
        int endIndex = ctx.input.crc.endIndex;
        bool bigEndian = ctx.input.crc.bigEndian;
        SAKCrcInterface crcInterface;
        QByteArray crcBytes = crcInterface.calculateBytes(
            bytes, arithmetic, startIndex, endIndex, bigEndian);
        input.append(crcBytes);
    }

    bytes.prepend(prefixData);
    bytes.append(suffixData);
    mToolBox->getInputAnalyzerTool()->inputBytes(bytes, QJsonObject());
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

QString SAKToolBoxUi::settingsGroup()
{
    if (mCommunicationToolType == SAKToolFactory::SerialportTool) {
        return "SerialportToolBox";
    } else if (mCommunicationToolType == SAKToolFactory::UdpClientTool) {
        return "UdpClientToolBox";
    } else if (mCommunicationToolType == SAKToolFactory::UdpServerTool) {
        return "UdpClientToolBox";
    }  else if (mCommunicationToolType == SAKToolFactory::TcpClientTool) {
        return "UdpServerToolBox";
    } else if (mCommunicationToolType == SAKToolFactory::TcpServerTool) {
        return "TcpServerToolBox";
    }  else if (mCommunicationToolType == SAKToolFactory::WebSocketClientTool) {
        return "WebSocketClientToolBox";
    } else if (mCommunicationToolType == SAKToolFactory::WebSocketServerTool) {
        return "WebSocketServerToolBox";
    } else if (mCommunicationToolType == SAKToolFactory::BleCentralTool) {
        return "BleCentralToolBox";
    } else {
        qCWarning(mLoggingCategory) << "Unknow type of communication tool ui:"
                                    << mCommunicationToolType;
        return  "UnknowToolBox";
    }
}

void SAKToolBoxUi::onIsWorkingChanged()
{
    bool isWorking = mToolBox->isWorking();
    ui->pushButtonCommunicationOpen->setEnabled(true);
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

void SAKToolBoxUi::onBytesWritten(const QByteArray &bytes,
                                  const QVariant &context)
{
    if (!ui->checkBoxOutputTx->isChecked()) {
        return;
    }

    output2ui(bytes, context, false);
}

void SAKToolBoxUi::onBytesRead(const QByteArray &bytes,
                               const QVariant &context)
{
    if (!ui->checkBoxOutputRx->isChecked()) {
        return;
    }

    output2ui(bytes, context, true);
}

void SAKToolBoxUi::init()
{
    mSettingsKey.tabIndex = settingsGroup() + "/tabIndex";

    initUi();
    initSettings();
    initSignals();
    initTools();

    onIsWorkingChanged();
}

void SAKToolBoxUi::initUi()
{
    initUiCommunication();
    initUiInput();
    initUiOutput();

    onComboBoxInputFormatActivated();
    QSettings *settings = SAKSettings::instance();
    int index = settings->value(mSettingsKey.tabIndex).toInt();
    if (index >= 0 && index < ui->tabWidget->count()) {
        ui->tabWidget->setCurrentIndex(index);
    }
}

void SAKToolBoxUi::initUiCommunication()
{
    // Setup communication tool.
    mCommunicationToolUi = communiticationToolUi(mCommunicationToolType);
    if (!mCommunicationToolUi) {
        qCWarning(mLoggingCategory) << "mCommunicationToolUi is nullptr";
        return;
    }

    // Clear widget.
    auto l = ui->widgetCommunicationToolUi->layout();
    if (l) {
        while (!l->isEmpty()) {
            auto item = l->itemAt(0);
            l->removeItem(item);
        }
    } else {
        ui->widgetCommunicationToolUi->setLayout(new QHBoxLayout());
        auto lt = ui->widgetCommunicationToolUi->layout();
        lt->setContentsMargins(0, 0, 0, 0);
    }

    ui->widgetCommunicationToolUi->layout()->addWidget(mCommunicationToolUi);
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
    mCommunicationToolUi->initialize(mCommunicationTool,
                                     settingsGroup() + "/communication");
}

void SAKToolBoxUi::initSettingsInput()
{
    QString group = settingsGroup();
    ui->comboBoxInputFormat->setGroupKey(group + "/input", "fromat");
    ui->comboBoxInputText->setGroupKey(group + "/input", "text", false);
}

void SAKToolBoxUi::initSettingsOutput()
{
    QString group = settingsGroup();
    ui->comboBoxOutputFormat->setGroupKey(group + "/output", "outputFormat");
    ui->checkBoxOutputRx->setGroupKey(group + "/output", "outputRx");
    ui->checkBoxOutputTx->setGroupKey(group + "/output", "outputTx");
    ui->checkBoxOutputDate->setGroupKey(group + "/output", "outputDate");
    ui->checkBoxOutputTime->setGroupKey(group + "/output", "outputTime");
    ui->checkBoxOutputMs->setGroupKey(group + "/output", "outputMs");
    ui->checkBoxOutputWrap->setGroupKey(group + "/output", "outputWrap");

#if 1
    ui->checkBoxOutputRx->setChecked(true);
    ui->checkBoxOutputTx->setChecked(true);
#endif
}

void SAKToolBoxUi::initSignals()
{
    initSignalsCommunication();
    initSignalsInput();
    initSignalsOutput();
    initSignalsTools();

    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &SAKToolBoxUi::onTabWidgetCurrentChanged);
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

void SAKToolBoxUi::initSignalsTools()
{
    connect(mToolBox, &SAKToolBox::isWorkingChanged,
            this, &SAKToolBoxUi::onIsWorkingChanged);

    connect(mCommunicationTool, &SAKCommunicationTool::bytesInputted,
            this, &SAKToolBoxUi::onBytesWritten);

    auto outputAnalyzer = mToolBox->getOutputAnalyzerTool();
    connect(outputAnalyzer, &SAKAnalyzerTool::bytesOutputted,
            this, &::SAKToolBoxUi::onBytesRead);
}

void SAKToolBoxUi::initTools()
{
    mToolBox->getInputMaskerTool()->setToolName("InputMasker");
    mToolBox->getOutputMaskerTool()->setToolName("OutputMasker");
    mToolBox->getInputAnalyzerTool()->setToolName("InputAnalyzer");
    mToolBox->getOutputAnalyzerTool()->setToolName("OutputAnalyzer");
    mToolBoxUiParameters->initialize(mToolBox, settingsGroup());
    mToolBox->getRxVelometerTool()->setToolName("Rx:");
    mToolBox->getTxVelometerTool()->setToolName("Tx:");
    ui->widgetRxVelometer->initialize(mToolBox->getRxVelometerTool(),
                                      settingsGroup());
    ui->widgetTxVelometer->initialize(mToolBox->getTxVelometerTool(),
                                      settingsGroup());

    mEmitterToolUi = new SAKEmitterToolUi();
    ui->tabEmiter->setLayout(new QVBoxLayout());
    ui->tabEmiter->layout()->addWidget(mEmitterToolUi);
    mEmitterToolUi->initialize(mToolBox->getEmitterTool(),
                               settingsGroup());

    mResponserToolUi = new SAKResponserToolUi();
    ui->tabResponser->setLayout(new QVBoxLayout());
    ui->tabResponser->layout()->addWidget(mResponserToolUi);
    mResponserToolUi->initialize(mToolBox->getResponserTool(),
                                 settingsGroup());

    mPrestorerToolUi = new SAKPrestorerToolUi();
    ui->tabPrestorer->setLayout(new QVBoxLayout());
    ui->tabPrestorer->layout()->addWidget(mPrestorerToolUi);
    mPrestorerToolUi->initialize(mToolBox->getPrestorerTool(),
                                 settingsGroup());
}

void SAKToolBoxUi::onTabWidgetCurrentChanged(int index)
{
    if (mSettingsKey.tabIndex.isEmpty()) {
        return;
    }

    SAKSettings::instance()->setValue(mSettingsKey.tabIndex, index);
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
    if (ui->comboBoxInputText->currentText().isEmpty()) {
        qCInfo(mLoggingCategory) << "Input text is empty,"
                                    " the operation will be ignored!";
        QApplication::beep();
        ui->comboBoxInputText->setFocus();
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
    auto lineEdit = ui->comboBoxInputText->lineEdit();
    SAKUiInterface::setValidator(lineEdit, format);
}
