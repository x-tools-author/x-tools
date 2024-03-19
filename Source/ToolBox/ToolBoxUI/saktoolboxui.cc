/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolboxui.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaEnum>
#include <QRegularExpression>

#include "sakcommunicationtool.h"
#include "sakcrcinterface.h"
#include "sakdatastructure.h"
#include "sakemittertoolui.h"
#include "sakinterface.h"
#include "sakprestorertoolui.h"
#include "sakresponsertoolui.h"
#include "sakserialporttoolui.h"
#include "sakserialporttransmittertoolui.h"
#include "saksettings.h"
#include "saksocketclienttoolui.h"
#include "saksocketservertoolui.h"
#include "saktcptransmittertoolti.h"
#include "saktoolboxuicommunicationmenu.h"
#include "saktoolboxuiinputmenu.h"
#include "saktoolboxuioutputmenu.h"
#include "saktoolfactory.h"
#include "sakudptransmittertoolui.h"
#include "sakuiinterface.h"
#include "sakwebsockettransmittertoolui.h"
#include "ui_saktoolboxui.h"

#ifdef SAK_IMPORT_MODULE_BLUETOOTH
#include "sakblecentraltoolui.h"
#endif

SAKToolBoxUi::SAKToolBoxUi(QWidget* parent)
    : QWidget{parent}
    , ui(new Ui::SAKToolBoxUi)
{
    ui->setupUi(this);
    m_toolBox = new SAKToolBox(this);

    m_cycleSendingTimer = new QTimer(this);
    connect(m_cycleSendingTimer, &QTimer::timeout, this, [=]() {
        if (ui->comboBoxInputText->currentText().isEmpty()) {
            setDefaultText();
        }

        try2send();
    });
}

SAKToolBoxUi::~SAKToolBoxUi()
{
    delete ui;
}

QList<int> SAKToolBoxUi::supportedCommunicationTools()
{
    QList<int> list;
    list << SAKToolFactory::SerialportTool
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
         << SAKToolFactory::BleCentralTool
#endif
         << SAKToolFactory::UdpClientTool << SAKToolFactory::UdpServerTool
         << SAKToolFactory::TcpClientTool << SAKToolFactory::TcpServerTool
         << SAKToolFactory::WebSocketClientTool << SAKToolFactory::WebSocketServerTool;
    return list;
}

QString SAKToolBoxUi::communicationToolName(int type)
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

QIcon SAKToolBoxUi::communicationToolIcon(int type)
{
    QString fileName;
    if (type == SAKToolFactory::SerialportTool) {
        fileName = ":/Resources/Icon/IconSerialPort.svg";
    } else if (type == SAKToolFactory::UdpClientTool) {
        fileName = ":/Resources/Icon/IconUdpClient.svg";
    } else if (type == SAKToolFactory::UdpServerTool) {
        fileName = ":/Resources/Icon/IconUdpServer.svg";
    } else if (type == SAKToolFactory::TcpClientTool) {
        fileName = ":/Resources/Icon/IconTcpClient.svg";
    } else if (type == SAKToolFactory::TcpServerTool) {
        fileName = ":/Resources/Icon/IconTcpServer.svg";
    } else if (type == SAKToolFactory::WebSocketClientTool) {
        fileName = ":/Resources/Icon/IconWebSocketClient.svg";
    } else if (type == SAKToolFactory::WebSocketServerTool) {
        fileName = ":/Resources/Icon/IconWebSocketServer.svg";
    } else if (type == SAKToolFactory::BleCentralTool) {
        fileName = ":/Resources/Icon/IconBlueTooth.svg";
    }

    QIcon icon(fileName);
    return icon;
}

void SAKToolBoxUi::initialize(int type)
{
    if (m_communication) {
        m_communication->deleteLater();
        m_communication = nullptr;
    }

    m_toolBox->initialize(type);
    m_communicationType = type;

    QString toolName = communicationToolName(type);
    QIcon icon = communicationToolIcon(type);
    setWindowTitle(toolName);
    setWindowIcon(icon);

    m_communication = m_toolBox->getCommunicationTool();
    if (!m_communication) {
        qWarning() << "initializing failed, "
                                       "tool box is invaliad!";
        return;
    }

    init();
}

SAKCommunicationToolUi* SAKToolBoxUi::communicationToolUi(int type)
{
    SAKCommunicationToolUi* w = nullptr;
    if (type == SAKToolFactory::SerialportTool) {
        w = new SAKSerialPortToolUi();
    } else if (type == SAKToolFactory::UdpClientTool) {
        w = new SAKSocketClientToolUi();
    } else if (type == SAKToolFactory::UdpServerTool) {
        w = new SAKSocketServerToolUi();
    } else if (type == SAKToolFactory::TcpClientTool) {
        w = new SAKSocketClientToolUi();
    } else if (type == SAKToolFactory::TcpServerTool) {
        w = new SAKSocketServerToolUi();
    } else if (type == SAKToolFactory::WebSocketClientTool) {
        w = new SAKSocketClientToolUi();
    } else if (type == SAKToolFactory::WebSocketServerTool) {
        w = new SAKSocketServerToolUi();
    }
#ifdef SAK_IMPORT_MODULE_BLUETOOTH
    else if (type == SAKToolFactory::BleCentralTool) {
        w = new SAKBleCentralToolUi();
    }
#endif
    else {
        qWarning() << "unknow type of communication tool ui!";
    }

    return w;
}

void SAKToolBoxUi::try2send()
{
    auto ctx = m_inputMenu->parameters();
    int prefix = ctx.prefix;
    int suffix = ctx.suffix;
    int esc = ctx.escapeCharacter;

    QByteArray prefixData = SAKDataStructure::affixesData(prefix);
    QByteArray suffixData = SAKDataStructure::affixesData(suffix);

    QString input = ui->comboBoxInputText->currentText();
    input = SAKDataStructure::cookedString(esc, input);
    int format = ui->comboBoxInputFormat->currentData().toInt();
    QByteArray bytes = SAKInterface::string2array(input, format);
    if (ctx.appendCrc) {
        QByteArray b = calculateCrc(bytes);
        bytes.append(b);
    }

    bytes.prepend(prefixData);
    bytes.append(suffixData);
    m_toolBox->getCommunicationTool()->inputBytes(bytes);
}

QString SAKToolBoxUi::dateTimeFormat()
{
    static QString dateFormat = QLocale::system().dateFormat();

    QString dateTimeFormat = "";
    bool d = ui->checkBoxOutputDate->isChecked();
    bool t = ui->checkBoxOutputTime->isChecked();
    bool m = ui->checkBoxOutputMs->isChecked();

    if ((!d) && (!t) && (!m)) {
        return QString("");
    }

    if (d && t && m) {
        dateTimeFormat = dateFormat + " hh:mm:ss.zzz";
    } else if (d && t) {
        dateTimeFormat = dateFormat + " hh:mm:ss";
    } else if (t && m) {
        dateTimeFormat = " hh:mm:ss.zzz";
    } else if (d && m) {
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

void SAKToolBoxUi::output2ui(const QByteArray& bytes, const QString& flag, bool isRx)
{
    int format = ui->comboBoxOutputFormat->currentData().toInt();
    QString str = SAKInterface::arrayToString(bytes, format);

    if (!str.contains(m_outputMenu->filter())) {
        return;
    }

    QString dt = dateTimeFormat();
    QString flags = isRx ? "Rx" : "Tx";
    QString color = isRx ? "red" : "blue";

    flags = QString("<font color=%1>%2</font>").arg(color, flag);
    QString info;
    if (dt.isEmpty()) {
        info = QString("[%1]").arg(flags);
    } else {
        dt = QString("<font color=silver>%1</font>").arg(dt);
        info = QString("[%1 %2]").arg(dt, flags);
    }

    info = QString("<font color=silver>%1</font>").arg(info);

    ui->textBrowserOutput->append(info + " " + str);
}

QString SAKToolBoxUi::settingsGroup()
{
    if (m_communicationType == SAKToolFactory::SerialportTool) {
        return "SerialportToolBox";
    } else if (m_communicationType == SAKToolFactory::UdpClientTool) {
        return "UdpClientToolBox";
    } else if (m_communicationType == SAKToolFactory::UdpServerTool) {
        return "UdpServerTool";
    } else if (m_communicationType == SAKToolFactory::TcpClientTool) {
        return "TcpClientTool";
    } else if (m_communicationType == SAKToolFactory::TcpServerTool) {
        return "TcpServerToolBox";
    } else if (m_communicationType == SAKToolFactory::WebSocketClientTool) {
        return "WebSocketClientToolBox";
    } else if (m_communicationType == SAKToolFactory::WebSocketServerTool) {
        return "WebSocketServerToolBox";
    } else if (m_communicationType == SAKToolFactory::BleCentralTool) {
        return "BleCentralToolBox";
    } else {
        qWarning() << "unknow type of communication tool ui:" << m_communicationType;
        return "UnknowToolBox";
    }
}

QByteArray SAKToolBoxUi::calculateCrc(const QByteArray& bytes, bool fixedOriginOrder)
{
    auto ctx = m_inputMenu->parameters();
    QByteArray inputBytes = bytes;
    if (inputBytes.isEmpty()) {
        int format = ui->comboBoxInputFormat->currentData().toInt();
        QString input = ui->comboBoxInputText->currentText();
        int esc = ctx.escapeCharacter;

        input = SAKDataStructure::cookedString(esc, input);
        inputBytes = SAKInterface::string2array(input, format);
    }

    int algorithm = ctx.algorithm;
    int startIndex = ctx.startIndex;
    int endIndex = ctx.endIndex;
    bool bigEndian = fixedOriginOrder ? true : ctx.bigEndian;
    SAKCrcInterface crci;
    QByteArray crc = crci.calculateBytes(inputBytes, algorithm, startIndex, endIndex, bigEndian);
    return crc;
}

void SAKToolBoxUi::setDefaultText()
{
    QByteArray ba("(null)");
    int format = ui->comboBoxInputFormat->currentData().toInt();
    QString str = SAKInterface::arrayToString(ba, format);
    ui->comboBoxInputText->setCurrentText(str);
}

void SAKToolBoxUi::onIsWorkingChanged()
{
    bool isWorking = m_toolBox->isWorking();
    ui->pushButtonInputSend->setEnabled(isWorking);
    ui->comboBoxInputIntervel->setEnabled(isWorking);
    QTimer::singleShot(1000, this, [=]() { ui->pushButtonCommunicationOpen->setEnabled(true); });

    if (isWorking) {
        ui->pushButtonCommunicationOpen->setText(tr("Close"));
    } else {
        ui->pushButtonCommunicationOpen->setText(tr("Open"));
    }

    if (!isWorking) {
        m_cycleSendingTimer->stop();
    }
}

void SAKToolBoxUi::onBytesWritten(const QByteArray& bytes, const QString& from)
{
    if (!ui->checkBoxOutputTx->isChecked()) {
        return;
    }

    output2ui(bytes, from, false);
}

void SAKToolBoxUi::onBytesRead(const QByteArray& bytes, const QString& to)
{
    if (!ui->checkBoxOutputRx->isChecked()) {
        return;
    }

    output2ui(bytes, to, true);
}

void SAKToolBoxUi::onInputTextChanged()
{
    QByteArray b = calculateCrc(QByteArray(), true);
    QString crc = QString::fromLatin1(b.toHex());
    crc = "0x" + crc.toUpper();
    ui->labelCrc->setText(crc);
    ui->labelAlgorithm->setText(m_inputMenu->parameters().algorithmName);
}

void SAKToolBoxUi::init()
{
    m_settingsKey.tabIndex = settingsGroup() + "/tabIndex";
    m_settingsKey.items = settingsGroup() + "/items";
    m_settingsKey.transmitterTabIndex = settingsGroup() + "/transmitterTabIndex";

    initUi();
    initSettings();
    initSignals();
    initTools();

    onIsWorkingChanged();
    onInputTextChanged();
    onComboBoxInputFormatActivated();
}

void SAKToolBoxUi::initUi()
{
    initUiCommunication();
    initUiInput();
    initUiOutput();

    QSettings* settings = SAKSettings::instance();
    int index = settings->value(m_settingsKey.tabIndex).toInt();
    if (index >= 0 && index < ui->tabWidget->count()) {
        ui->tabWidget->setCurrentIndex(index);
    }
}

void SAKToolBoxUi::initUiCommunication()
{
    // Setup communication tool.
    m_communicationUi = communicationToolUi(m_communicationType);
    if (!m_communicationUi) {
        qWarning() << "mCommunicationToolUi is nullptr";
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

    ui->widgetCommunicationToolUi->layout()->addWidget(m_communicationUi);
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

    QString hex = SAKSettings::instance()->value(m_settingsKey.items).toString();
    if (!hex.isEmpty()) {
        QString json = QString::fromUtf8(QByteArray::fromHex(hex.toLatin1()));
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
        QJsonArray arr = jsonDoc.array();
        for (int i = 0; i < arr.count(); i++) {
            QJsonObject obj = arr.at(i).toObject();
            QString text = obj.value("text").toString();
            int format = obj.value("data").toInt();
            ui->comboBoxInputText->addItem(text, format);
        }
    }

    m_inputMenu = new SAKToolBoxUiInputMenu(settingsGroup(), this);
    connect(m_inputMenu,
            &SAKToolBoxUiInputMenu::parametersChanged,
            this,
            &SAKToolBoxUi::onInputTextChanged);
    ui->pushButtonInputSettings->setMenu(m_inputMenu);
}

void SAKToolBoxUi::initUiOutput()
{
    ui->checkBoxOutputRx->setChecked(true);
    ui->checkBoxOutputTx->setChecked(true);
    ui->textBrowserOutput->document()->setMaximumBlockCount(2000);

    m_outputMenu = new SAKToolBoxUiOutputMenu(settingsGroup(),
                                              ui->textBrowserOutput->document(),
                                              this);
    ui->pushButtonOutputSettings->setMenu(m_outputMenu);
}

void SAKToolBoxUi::initSettings()
{
    initSettingsCommunication();
    initSettingsInput();
    initSettingsOutput();
}

void SAKToolBoxUi::initSettingsCommunication()
{
    const QString key = settingsGroup() + "/communication";
    m_communicationUi->initialize(m_communication, key);
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
    ui->checkBoxOutputWrap->setChecked(true);
#endif
}

void SAKToolBoxUi::initSignals()
{
    initSignalsCommunication();
    initSignalsInput();
    initSignalsOutput();

    connect(ui->tabWidget,
            &QTabWidget::currentChanged,
            this,
            &SAKToolBoxUi::onTabWidgetCurrentChanged);
}

void SAKToolBoxUi::initSignalsCommunication()
{
    connect(ui->pushButtonCommunicationOpen,
            &QPushButton::clicked,
            this,
            &SAKToolBoxUi::onPushButtonCommunicationOpenClicked);
}

void SAKToolBoxUi::initSignalsInput()
{
    connect(ui->comboBoxInputIntervel,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this,
            &SAKToolBoxUi::onComboBoxInputIntervalActivated);
    connect(ui->pushButtonInputSend,
            &QPushButton::clicked,
            this,
            &SAKToolBoxUi::onPushButtonInputSendClicked);
    connect(ui->comboBoxInputFormat,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this,
            &SAKToolBoxUi::onComboBoxInputFormatActivated);
    connect(ui->comboBoxInputText,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this,
            &SAKToolBoxUi::onComboBoxInputTextActivated);
    connect(ui->comboBoxInputText,
            &QComboBox::currentTextChanged,
            this,
            &SAKToolBoxUi::onInputTextChanged);
}

void SAKToolBoxUi::initSignalsOutput()
{
    connect(ui->checkBoxOutputWrap,
            &QCheckBox::clicked,
            this,
            &SAKToolBoxUi::onCheckBoxOutputWrapClicked);
}

void SAKToolBoxUi::initTools()
{
    m_communicationMenu = new SAKToolBoxUiCommunicationMenu(this);
    m_communicationMenu->initialize(m_toolBox, settingsGroup());
    ui->pushButtonCommunicationSettings->setMenu(m_communicationMenu);

    auto rxVelometer = m_toolBox->getRxVelometerTool();
    auto txVelometer = m_toolBox->getTxVelometerTool();
    auto rxS = m_toolBox->getRxStatisticianTool();
    auto txS = m_toolBox->getTxStatisticianTool();
    const QString rxVelometerGroup = settingsGroup() + "/rxVelometer";
    const QString txVelometerGroup = settingsGroup() + "/txVelometer";
    const QString rxSGroup = settingsGroup() + "/rxStatistician";
    const QString txSGroup = settingsGroup() + "/txStatistician";
    ui->widgetRxVelometer->initialize(rxVelometer, rxVelometerGroup);
    ui->widgetTxVelometer->initialize(txVelometer, txVelometerGroup);
    ui->widgetRxStatistician->initialize(rxS, rxSGroup);
    ui->widgetTxStatistician->initialize(txS, txSGroup);

    m_emitterUi = new SAKEmitterToolUi();
    ui->tabEmiter->setLayout(new QVBoxLayout());
    ui->tabEmiter->layout()->addWidget(m_emitterUi);
    m_emitterUi->initialize(m_toolBox->getEmitterTool(), settingsGroup() + "/emitter");

    m_responserUi = new SAKResponserToolUi();
    ui->tabResponser->setLayout(new QVBoxLayout());
    ui->tabResponser->layout()->addWidget(m_responserUi);
    m_responserUi->initialize(m_toolBox->getResponserTool(), settingsGroup() + "/responser");

    m_prestorerUi = new SAKPrestorerToolUi();
    ui->tabPrestorer->setLayout(new QVBoxLayout());
    ui->tabPrestorer->layout()->addWidget(m_prestorerUi);
    m_prestorerUi->initialize(m_toolBox->getPrestorerTool(), settingsGroup() + "/prestorer");

    m_tcpTransmitterUi = new SAKTcpTransmitterToolUi(this);
    m_tcpTransmitterUi->initialize(m_toolBox->getTcpTransmitterTool(),
                                   settingsGroup() + "/tcpTransmitter");
    m_udpTransmitterUi = new SAKUdpTransmitterToolUi(this);
    m_udpTransmitterUi->initialize(m_toolBox->getUdpTransmitterTool(),
                                   settingsGroup() + "/udpTransmitter");
    m_webSocketTransmitterUi = new SAKWebSocketTransmitterToolUi(this);
    m_webSocketTransmitterUi->initialize(m_toolBox->getWebSocketTransmitterTool(),
                                         settingsGroup() + "/webSocketTransmitter");
    m_serialPortTransmitterUi = new SAKSerialPortTransmitterToolUi(this);
    m_serialPortTransmitterUi->initialize(m_toolBox->getSerialPortTransmitterTool(),
                                          settingsGroup() + "/serialPortTransmitter");

    m_tcpTransmitterUi->layout()->setContentsMargins(9, 9, 9, 9);
    m_udpTransmitterUi->layout()->setContentsMargins(9, 9, 9, 9);
    m_webSocketTransmitterUi->layout()->setContentsMargins(9, 9, 9, 9);
    m_serialPortTransmitterUi->layout()->setContentsMargins(9, 9, 9, 9);

    ui->tabWidgetTransmitter->clear();
    ui->tabWidgetTransmitter->addTab(m_serialPortTransmitterUi,
                                     tr("SerialPort", "Transmitter", __LINE__));
    ui->tabWidgetTransmitter->addTab(m_udpTransmitterUi, tr("UDP"));
    ui->tabWidgetTransmitter->addTab(m_tcpTransmitterUi, tr("TCP"));
    ui->tabWidgetTransmitter->addTab(m_webSocketTransmitterUi, tr("WebSocket"));

    const QString key = m_settingsKey.transmitterTabIndex;
    int tabIndex = SAKSettings::instance()->value(key).toInt();
    if ((tabIndex >= 0) && (tabIndex < ui->tabWidgetTransmitter->count())) {
        ui->tabWidgetTransmitter->setCurrentIndex(tabIndex);
    }

    connect(ui->tabWidgetTransmitter, &QTabWidget::currentChanged, this, [=](int index) {
        SAKSettings::instance()->setValue(key, index);
    });

    connect(m_toolBox, &SAKToolBox::isWorkingChanged, this, &SAKToolBoxUi::onIsWorkingChanged);
    connect(m_toolBox, &SAKToolBox::errorOccurred, this, [=](const QString& err) {
        QMessageBox::warning(this, tr("Error Occured"), err);
    });

    connect(m_communication,
            &SAKCommunicationTool::bytesWritten,
            this,
            &SAKToolBoxUi::onBytesWritten);
    connect(m_communication, &SAKCommunicationTool::bytesRead, this, &SAKToolBoxUi::onBytesRead);

    ui->pushButtonPrestorer->setMenu(m_prestorerUi->menu());
}

void SAKToolBoxUi::onTabWidgetCurrentChanged(int index)
{
    if (m_settingsKey.tabIndex.isEmpty()) {
        return;
    }

    SAKSettings::instance()->setValue(m_settingsKey.tabIndex, index);
}

void SAKToolBoxUi::onPushButtonCommunicationOpenClicked()
{
    ui->pushButtonCommunicationOpen->setEnabled(false);
    if (m_toolBox->isWorking()) {
        m_toolBox->close();
    } else {
        m_toolBox->open();
    }
}

void SAKToolBoxUi::onCheckBoxOutputWrapClicked()
{
    if (ui->checkBoxOutputWrap->isChecked()) {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::WrapAnywhere);
    } else {
        ui->textBrowserOutput->setWordWrapMode(QTextOption::NoWrap);
    }
}

void SAKToolBoxUi::onPushButtonInputSendClicked()
{
    if (ui->comboBoxInputText->currentText().isEmpty()) {
        qInfo() << "input text is empty,"
                                    "the text will be set as (null)";
        QApplication::beep();
        ui->comboBoxInputText->setFocus();

        setDefaultText();
    }

    QString text = ui->comboBoxInputText->currentText();
    int format = ui->comboBoxInputFormat->currentData().toInt();
    bool isExist = false;
    for (int i = 0; i < ui->comboBoxInputText->count(); i++) {
        QString t = ui->comboBoxInputText->itemText(i);
        int f = ui->comboBoxInputText->itemData(i).toInt();
        if ((text == t) && (format == f)) {
            isExist = true;
            break;
        }
    }

    if (!isExist) {
        ui->comboBoxInputText->addItem(text, format);
        while (ui->comboBoxInputText->count() > 10) {
            ui->comboBoxInputText->removeItem(0);
        }

        QJsonArray arr;
        QJsonObject obj;
        for (int i = 0; i < ui->comboBoxInputText->count(); i++) {
            QString t = ui->comboBoxInputText->itemText(i);
            int f = ui->comboBoxInputText->itemData(i).toInt();
            obj.insert("text", t);
            obj.insert("data", f);
            arr.append(obj);
        }

        QJsonDocument jsonDoc;
        jsonDoc.setArray(arr);
        QByteArray json = jsonDoc.toJson();
        QString hex = QString::fromLatin1(json.toHex());
        SAKSettings::instance()->setValue(m_settingsKey.items, hex);
    }

    try2send();
}

void SAKToolBoxUi::onComboBoxInputIntervalActivated()
{
    int interval = ui->comboBoxInputIntervel->currentText().toInt();
    interval = interval < 10 ? 10 : interval;
    qInfo() << "start sending automatically, the interval is:" << interval;

    if (ui->comboBoxInputIntervel->currentIndex() == 0) {
        m_cycleSendingTimer->stop();
    } else {
        m_cycleSendingTimer->start(interval);
    }
}

void SAKToolBoxUi::onComboBoxInputFormatActivated()
{
    int format = ui->comboBoxInputFormat->currentData().toInt();
    auto lineEdit = ui->comboBoxInputText->lineEdit();
    ui->comboBoxInputText->clear();
    SAKUiInterface::setValidator(lineEdit, format);
}

void SAKToolBoxUi::onComboBoxInputTextActivated()
{
    int format = ui->comboBoxInputText->currentData().toInt();
    ui->comboBoxInputFormat->setCurrentIndexFromData(format);
    onComboBoxInputFormatActivated();
}
