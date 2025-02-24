/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ModbusAssistant.h"
#include "ui_ModbusAssistant.h"

#include <QAbstractSocket>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QGroupBox>
#include <QHostAddress>
#include <QLineEdit>
#include <QMessageBox>
#include <QModbusServer>
#include <QModbusTcpClient>
#include <QModbusTcpServer>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QPushButton>
#include <QRegularExpression>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTextBrowser>
#include <QtEndian>

#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>

#include "Common/xTools.h"
#include "IO/xIO.h"
#include "ModbusFactory.h"

#define RXCOLOR "green"
#define TXCOLOR "blue"
#define RXFLAG "rx:"
#define TXFLAG "tx:"
#define MAX_HISTORY_INDEX 9

struct ModbusSettingKeys
{
    const QString deviceIndex = "xToolsModbus/deviceIndex";

    const QString portName = "xToolsModbus/portName";
    const QString parity = "xToolsModbus/parity";
    const QString baudRate = "xToolsModbus/baudRate";
    const QString dataBits = "xToolsModbus/dataBits";
    const QString stopBits = "xToolsModbus/stopBits";
    const QString customBaudRate = "xToolsModbus/customBaudRate";

    const QString address = "xToolsModbus/address";
    const QString port = "xToolsModbus/port";
    const QString customAddress = "xToolsModbus/customAddress";

    const QString clientTimeout = "xToolsModbus/clientTimeout";
    const QString clientRepeatTime = "xToolsModbus/clientRepeatTime";

    const QString serverIsBusy = "xToolsModbus/serverIsBusy";
    const QString serverJustListen = "xToolsModbus/serverJustListen";
    const QString serverAddress = "xToolsModbus/serverAddress";

    const QString functionCode = "xToolsModbus/functionCode";
    const QString targetAddress = "xToolsModbus/targetAddress";
    const QString startAddress = "xToolsModbus/startAddress";
    const QString addressNumber = "xToolsModbus/addressNumber";
    const QString sendHistoryIndex = "xToolsModbus/sendHistoryIndex";
    const QString pdu = "xToolsModbus/pdu";
    const QString inputFormat = "xToolsModbus/inputFormat";
};

class ReadOnlyDelegate : public QStyledItemDelegate
{
public:
    ReadOnlyDelegate(QWidget *parent = Q_NULLPTR)
        : QStyledItemDelegate(parent)
    {}
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        Q_UNUSED(parent);
        Q_UNUSED(option);
        Q_UNUSED(index);
        return Q_NULLPTR;
    }
};

QList<quint16> vectorTolist(const QVector<quint16> &vector)
{
    QList<quint16> list;
    for (int i = 0; i < vector.length(); vector.count()) {
        list.append(vector.at(i));
    }
    return list;
}

QVector<quint16> listToVector(const QList<quint16> &list)
{
    QVector<quint16> vector;
    for (int i = 0; i < list.length(); list.count()) {
        vector.append(list.at(i));
    }
    return vector;
}

ModbusAssistant::ModbusAssistant(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::ModbusAssistant)
    , m_modbusDevice(Q_NULLPTR)
    , m_clientRegisterModel(Q_NULLPTR)
    , m_keyCtx(new ModbusSettingKeys)
    , m_textFormat(static_cast<int>(xTools::xIO::TextFormat::Dec))
{
    xTools::xTools &xTools = xTools::xTools::singleton();
    m_settings = xTools.settings();

    ui->setupUi(this);

    initComponents();
    initSettings();
    initSignals();

    updateUiState(false);
    onDeviceTypeChanged();
    updateClientTableView(m_textFormat, ui->comboBoxFormat->currentData().toInt());
    updateClientReadWriteButtonState();
    updateClientTableViewAddress(ui->tabViewClientRegisters, startAddress());
}

ModbusAssistant::~ModbusAssistant()
{
    delete ui;
}

void ModbusAssistant::initComponents()
{
    int dec = static_cast<int>(xTools::xIO::TextFormat::Dec);
    ui->comboBoxFormat->addItem(xTools::xIO::textFormatName(xTools::xIO::TextFormat::Dec), dec);
    int hex = static_cast<int>(xTools::xIO::TextFormat::Hex);
    ui->comboBoxFormat->addItem(xTools::xIO::textFormatName(xTools::xIO::TextFormat::Hex), hex);

    initComponentDevices();
    initComponentAddress();
    initComponentPortName();
    initComponentBaudRate();
    initComponentDataBits();
    initComponentStopBits();
    initComponentParity();
    initComponentFunctionCode();
    initComponentRegisterTableView();
    initComponentInput();
    initComponentRegisterTabWidget();
}

void ModbusAssistant::initComponentDevices()
{
    ui->comboBoxDeviceList->addItem(tr("RtuClient"), ModbusFactory::ModbusRtuSerialClient);
    ui->comboBoxDeviceList->addItem(tr("RtuServer"), ModbusFactory::ModbusRtuSerialServer);
    ui->comboBoxDeviceList->addItem(tr("TcpClient"), ModbusFactory::ModbusTcpClient);
    ui->comboBoxDeviceList->addItem(tr("TcpServer"), ModbusFactory::ModbusTcpServer);
}

void ModbusAssistant::initComponentAddress()
{
    ui->comboBoxAddress->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->comboBoxAddress->addItem(address.toString());
        }
    }
}

void ModbusAssistant::initComponentPortName()
{
    ui->comboBoxPortName->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui->comboBoxPortName->addItem(info.portName());
    }
}

void ModbusAssistant::initComponentBaudRate()
{
    ui->comboBoxBaudRate->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui->comboBoxBaudRate->addItem(QString::number(bd), bd);
    }
}

void ModbusAssistant::initComponentDataBits()
{
    ui->comboBoxDataBits->clear();
    ui->comboBoxDataBits->addItem("8", QSerialPort::Data8);
    ui->comboBoxDataBits->addItem("7", QSerialPort::Data7);
    ui->comboBoxDataBits->addItem("6", QSerialPort::Data6);
    ui->comboBoxDataBits->addItem("5", QSerialPort::Data5);
}

void ModbusAssistant::initComponentStopBits()
{
    ui->comboBoxStopBits->clear();
    ui->comboBoxStopBits->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->comboBoxStopBits->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->comboBoxStopBits->addItem("2", QSerialPort::TwoStop);
}

void ModbusAssistant::initComponentParity()
{
    ui->comboBoxParity->clear();
    ui->comboBoxParity->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui->comboBoxParity->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui->comboBoxParity->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui->comboBoxParity->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui->comboBoxParity->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void ModbusAssistant::initComponentFunctionCode()
{
    const QString str0x01 = tr("0x01-ReadCoils");
    const QString str0x02 = tr("0x02-ReadDiscreteInputs");
    const QString str0x03 = tr("0x03-ReadHoldingRegisters");
    const QString str0x04 = tr("0x04-ReadInputRegisters");
    const QString str0x05 = tr("0x05-WriteSingleCoil");
    const QString str0x06 = tr("0x06-WriteSingleRegister");
    const QString str0x0f = tr("0x0f-WriteMultipleCoils");
    const QString str0x10 = tr("0x10-WriteMultipleRegisters");

    int func0x01 = QModbusDataUnit::Coils;
    int func0x02 = QModbusDataUnit::DiscreteInputs;
    int func0x03 = QModbusDataUnit::HoldingRegisters;
    int func0x04 = QModbusDataUnit::InputRegisters;
    int func0x05 = QModbusDataUnit::Coils;
    int func0x06 = QModbusDataUnit::HoldingRegisters;
    int func0x0f = QModbusDataUnit::Coils;
    int func0x10 = QModbusDataUnit::HoldingRegisters;

    ui->comboBoxFunctionCode->addItem(str0x01, func0x01);
    ui->comboBoxFunctionCode->addItem(str0x02, func0x02);
    ui->comboBoxFunctionCode->addItem(str0x03, func0x03);
    ui->comboBoxFunctionCode->addItem(str0x04, func0x04);
    ui->comboBoxFunctionCode->addItem(str0x05, func0x05);
    ui->comboBoxFunctionCode->addItem(str0x06, func0x06);
    ui->comboBoxFunctionCode->addItem(str0x0f, func0x0f);
    ui->comboBoxFunctionCode->addItem(str0x10, func0x10);
}

void ModbusAssistant::initComponentRegisterTableView()
{
    m_clientView = createTableView(1, ui->tabViewClientRegisters);
    m_clientRegisterModel = qobject_cast<QStandardItemModel *>(m_clientView->model());
    //m_clientView->setItemDelegateForColumn(1, new DecDelegate(m_clientView));
}

void ModbusAssistant::initComponentInput()
{
#if 0
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui->comboBoxPdu->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
#endif
}

void ModbusAssistant::initComponentRegisterTabWidget()
{
    QTabWidget *tabWidget = ui->tabWidgetServerRegisters;
    QStringList titles = QStringList() << tr("Coils") << tr("DiscreteInputs")
                                       << tr("InputRegisters") << tr("HoldingRegisters");
    for (QString &title : titles) {
        QTableView *tableView = createTableView(UINT16_MAX + 1, Q_NULLPTR);
        tableView->verticalHeader()->hide();
        tabWidget->addTab(tableView, title);
    }
}

void ModbusAssistant::initSettings()
{
    initSettingsDevice();
    initSettingsNetwork();
    initSettingsSerialPort();
    initSettingsClient();
    initSettingsServer();
    initSettingsClientOperations();
    initSettingsInput();
    initSettingsInputControl();
}

void ModbusAssistant::initSettingsDevice()
{
    int deviceIndex = m_settings->value(m_keyCtx->deviceIndex).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui->comboBoxDeviceList->count()) {
        ui->comboBoxDeviceList->setCurrentIndex(deviceIndex);
    }
}

void ModbusAssistant::initSettingsNetwork()
{
    QString address = m_settings->value(m_keyCtx->address).toString();
    ui->comboBoxAddress->setCurrentText(address);

    QVariant portValiant = m_settings->value(m_keyCtx->port);
    int port = portValiant.toInt();
    if (portValiant.isNull()) {
        port = 502;
    }
    ui->spinBoxPort->setValue(port);
}

void ModbusAssistant::initSettingsSerialPort()
{
    auto SetComboBoxIndex = [](int index, QComboBox *cb) {
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    };

    QString portName = m_settings->value(m_keyCtx->portName).toString();
    int index = ui->comboBoxPortName->findText(portName);
    SetComboBoxIndex(index, ui->comboBoxPortName);

    index = m_settings->value(m_keyCtx->parity).toInt();
    SetComboBoxIndex(index, ui->comboBoxParity);

    QString bd = m_settings->value(m_keyCtx->baudRate).toString();
    ui->comboBoxBaudRate->setCurrentText(bd);

    index = m_settings->value(m_keyCtx->dataBits).toInt();
    SetComboBoxIndex(index, ui->comboBoxDataBits);

    index = m_settings->value(m_keyCtx->stopBits).toInt();
    SetComboBoxIndex(index, ui->comboBoxStopBits);
}

void ModbusAssistant::initSettingsClient()
{
    int timeout = m_settings->value(m_keyCtx->clientTimeout).toInt();
    ui->spinBoxTimeout->setValue(timeout < 100 ? 100 : timeout);

    const QString key = m_keyCtx->clientRepeatTime;
    int repeatTimes = m_settings->value(key).toInt();
    ui->spinBoxRepeatTime->setValue(repeatTimes);
}

void ModbusAssistant::initSettingsServer()
{
    bool isBusy = m_settings->value(m_keyCtx->serverIsBusy).toBool();
    ui->checkBoxDeviceBusy->setChecked(isBusy);

    QString key = m_keyCtx->serverJustListen;
    bool just_listen = m_settings->value(key).toBool();
    ui->checkBoxListenOnlyMode->setChecked(just_listen);

    int address = m_settings->value(m_keyCtx->serverAddress).toInt();
    ui->spinBoxServerAddress->setValue(address);
}

void ModbusAssistant::initSettingsClientOperations()
{
    int index = m_settings->value(m_keyCtx->functionCode).toInt();
    if (index >= 0 && index < ui->comboBoxFunctionCode->count()) {
        ui->comboBoxFunctionCode->setCurrentIndex(index);
    }

    int address = m_settings->value(m_keyCtx->targetAddress).toInt();
    ui->spinBoxDeviceAddress->setValue(address);

    int start = m_settings->value(m_keyCtx->startAddress).toInt();
    int format = m_settings->value(m_keyCtx->inputFormat).toInt();
    if (format == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
        ui->lineEditStartAddress->setText(QString::number(start));
    } else {
        QString str = QString("%1").arg(QString::number(start, 16), 4, '0');
        ui->lineEditStartAddress->setText(str);
    }

    int number = m_settings->value(m_keyCtx->addressNumber).toInt();
    ui->spinBoxQuantity->setValue(number);
}

void ModbusAssistant::initSettingsInput()
{
    ui->lineEditPdu->clear();
    QString pdu = m_settings->value(m_keyCtx->pdu).toString();
    ui->lineEditPdu->setText(pdu);
}

void ModbusAssistant::initSettingsInputControl()
{
    int dec = static_cast<int>(xTools::xIO::TextFormat::Dec);
    int format = m_settings->value(m_keyCtx->inputFormat, dec).toInt();
    int index = ui->comboBoxFormat->findData(format);
    if (index >= 0 && index < ui->comboBoxFormat->count()) {
        ui->comboBoxFormat->setCurrentIndex(index);
    }

    m_textFormat = format;
}

void ModbusAssistant::initSignals()
{
    initSignalsDevice();
    initSignalsNetworking();
    initSignalsSerialPort();
    initSignalsClient();
    initSignalsServer();
    initSignalsClientOperations();
    initSignalsInput();
    initSignalsInputControl();
}

void ModbusAssistant::initSignalsDevice()
{
    connect(ui->comboBoxDeviceList,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ModbusAssistant::onDeviceTypeChanged);
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &ModbusAssistant::onOpenClicked);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &ModbusAssistant::onCloseClicked);
}

void ModbusAssistant::initSignalsNetworking()
{
    connect(ui->comboBoxAddress,
            &QComboBox::currentTextChanged,
            this,
            &ModbusAssistant::onAddressChanged);
    connect(ui->spinBoxPort,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &ModbusAssistant::onPortChanged);
}

void ModbusAssistant::initSignalsSerialPort()
{
    connect(ui->comboBoxPortName,
            &QComboBox::currentTextChanged,
            this,
            &ModbusAssistant::onPortNameChanged);
    connect(ui->comboBoxParity,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ModbusAssistant::onParityChanged);
    connect(ui->comboBoxBaudRate,
            &QComboBox::currentTextChanged,
            this,
            &ModbusAssistant::onBaudRateChanged);
    connect(ui->comboBoxDataBits,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ModbusAssistant::onDataBitsChanged);
    connect(ui->comboBoxStopBits,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ModbusAssistant::onStopBistChanged);
}

void ModbusAssistant::initSignalsClient()
{
    connect(ui->spinBoxTimeout,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &ModbusAssistant::onClientTimeoutChanged);
    connect(ui->spinBoxRepeatTime,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &ModbusAssistant::onClientRepeatTimeChanged);
}

void ModbusAssistant::initSignalsServer()
{
    connect(ui->checkBoxDeviceBusy,
            &QCheckBox::clicked,
            this,
            &ModbusAssistant::onServerIsBusyChanged);
    connect(ui->checkBoxListenOnlyMode,
            &QCheckBox::clicked,
            this,
            &ModbusAssistant::onServerJustListenChanged);
    connect(ui->spinBoxServerAddress,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &ModbusAssistant::onServerAddressChanged);
}

void ModbusAssistant::initSignalsClientOperations()
{
    connect(ui->comboBoxFunctionCode,
            &QComboBox::currentTextChanged,
            this,
            &ModbusAssistant::onFunctionCodeChanged);
    connect(ui->spinBoxDeviceAddress,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &ModbusAssistant::onTargetAddressChanged);
    connect(ui->lineEditStartAddress,
            &QLineEdit::textChanged,
            this,
            &ModbusAssistant::onStartAddressChanged);
    connect(ui->spinBoxQuantity,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &ModbusAssistant::onAddressNumberChanged);
    connect(ui->pushButtonRead, &QPushButton::clicked, this, &ModbusAssistant::onReadClicked);
    connect(ui->pushButtonWrite, &QPushButton::clicked, this, &ModbusAssistant::onWriteClicked);
    connect(ui->send_, &QPushButton::clicked, this, &ModbusAssistant::onSendClicked);
}

void ModbusAssistant::initSignalsInput()
{
    connect(ui->lineEditPdu, &QLineEdit::textEdited, this, [this]() {
        QString pdu = ui->lineEditPdu->text();
        m_settings->setValue(m_keyCtx->pdu, pdu);
    });
}

void ModbusAssistant::initSignalsInputControl()
{
    connect(ui->comboBoxFormat,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &ModbusAssistant::onInputFormatChanged);
}

void ModbusAssistant::onErrorOccurred()
{
    outputMessage(m_modbusDevice->errorString(), true, "", "error");
    if (m_modbusDevice->error() == QModbusDevice::ConnectionError) {
        const QString msg = m_modbusDevice->errorString();
        onCloseClicked();
        QMessageBox::warning(this, tr("Error occured"), msg);
    }
}

void ModbusAssistant::onDeviceTypeChanged()
{
    int type = ui->comboBoxDeviceList->currentData().toInt();
    bool isSerial = (type == ModbusFactory::ModbusRtuSerialClient
                     || type == ModbusFactory::ModbusRtuSerialServer);
    bool isClient = (type == ModbusFactory::ModbusRtuSerialClient
                     || type == ModbusFactory::ModbusTcpClient);

    // Hide ui component first then show ui component,
    // or the window will be resize to the max size of default.
    if (isSerial) {
        ui->widgetNetworkSettings->setHidden(true);
        ui->widgetSerialPortSettings->setHidden(false);
    } else {
        ui->widgetSerialPortSettings->setHidden(true);
        ui->widgetNetworkSettings->setHidden(false);
    }

    if (isClient) {
        ui->widgetConsole->setVisible(true);
        ui->widgetServerParameters->setHidden(true);
        ui->widgetServerRegisters->setHidden(true);
        ui->widgetClientParameters->setHidden(false);
        ui->widgetClientRegisters->setHidden(false);
    } else {
        ui->widgetConsole->setVisible(false);
        ui->widgetClientRegisters->setHidden(true);
        ui->widgetClientParameters->setHidden(true);
        ui->widgetServerParameters->setHidden(false);
        ui->widgetServerRegisters->setHidden(false);
    }

    m_settings->setValue(m_keyCtx->deviceIndex, type);
}

void ModbusAssistant::onCloseClicked()
{
    ModbusFactory::Instance()->deleteModbusDevuce(&m_modbusDevice);
    updateUiState(false);
}

void ModbusAssistant::onOpenClicked()
{
    ui->pushButtonOpen->setEnabled(false);
    ModbusFactory::Instance()->deleteModbusDevuce(&m_modbusDevice);

    m_modbusDevice = createModbusDevice();

    if (ModbusFactory::Instance()->isServerDevice(m_modbusDevice)) {
        if (!updateServerMap(m_modbusDevice)) {
            ui->pushButtonOpen->setEnabled(true);
            qWarning() << "Can not reset server map!";
            return;
        }

        updateServerParameters();

        QModbusServer *server = qobject_cast<QModbusServer *>(m_modbusDevice);
        updateServerRegistersViews(m_textFormat, ui->comboBoxFormat->currentData().toInt());
        connect(server, &QModbusServer::dataWritten, this, &ModbusAssistant::onDateWritten);
    } else if (ModbusFactory::Instance()->isClientDevice(m_modbusDevice)) {
        updateClientParameters();
    } else {
        ui->pushButtonOpen->setEnabled(true);
        qWarning() << "Can not create modbus devices!";
        return;
    }

    connect(m_modbusDevice, &QModbusDevice::errorOccurred, this, &ModbusAssistant::onErrorOccurred);
    ModbusFactory *factory = ModbusFactory::Instance();
    bool connected = factory->connectDeivce(m_modbusDevice);
    if (!connected) {
        QString errStr = m_modbusDevice->errorString();
        QString info = tr("Can not open device: %1."
                          "Please check the parameters and try again!")
                           .arg(errStr);
        QMessageBox::warning(this, tr("Can not open device"), info);
    }

    updateUiState(connected);
}

void ModbusAssistant::onAddressChanged()
{
    m_settings->setValue(m_keyCtx->address, ui->comboBoxAddress->currentText());
}

void ModbusAssistant::onPortChanged()
{
    m_settings->setValue(m_keyCtx->port, ui->spinBoxPort->value());
}

void ModbusAssistant::onPortNameChanged()
{
    m_settings->setValue(m_keyCtx->portName, ui->comboBoxPortName->currentText());
}

void ModbusAssistant::onParityChanged()
{
    m_settings->setValue(m_keyCtx->parity, ui->comboBoxParity->currentIndex());
}

void ModbusAssistant::onBaudRateChanged()
{
    m_settings->setValue(m_keyCtx->baudRate, ui->comboBoxBaudRate->currentText());
}

void ModbusAssistant::onDataBitsChanged()
{
    m_settings->setValue(m_keyCtx->dataBits, ui->comboBoxDataBits->currentIndex());
}

void ModbusAssistant::onStopBistChanged()
{
    m_settings->setValue(m_keyCtx->stopBits, ui->comboBoxStopBits->currentIndex());
}

void ModbusAssistant::onInvokeRefresh()
{
    initComponentPortName();
}

void ModbusAssistant::onClientTimeoutChanged()
{
    m_settings->setValue(m_keyCtx->clientTimeout, ui->spinBoxTimeout->value());
    updateClientParameters();
}

void ModbusAssistant::onClientRepeatTimeChanged()
{
    m_settings->setValue(m_keyCtx->clientRepeatTime, ui->spinBoxRepeatTime->value());
    updateClientParameters();
}

void ModbusAssistant::onServerIsBusyChanged()
{
    m_settings->setValue(m_keyCtx->serverIsBusy, ui->checkBoxDeviceBusy->isChecked());
    updateServerParameters();
}

void ModbusAssistant::onServerJustListenChanged()
{
    m_settings->setValue(m_keyCtx->serverJustListen, ui->checkBoxListenOnlyMode->isChecked());
    updateServerParameters();
}

void ModbusAssistant::onServerAddressChanged()
{
    m_settings->setValue(m_keyCtx->serverAddress, ui->spinBoxServerAddress->value());
    updateServerParameters();
}

void ModbusAssistant::onFunctionCodeChanged()
{
    m_settings->setValue(m_keyCtx->functionCode, ui->comboBoxFunctionCode->currentIndex());
    updateClientReadWriteButtonState();
}

void ModbusAssistant::onTargetAddressChanged()
{
    m_settings->setValue(m_keyCtx->targetAddress, ui->spinBoxDeviceAddress->value());
}

void ModbusAssistant::onStartAddressChanged()
{
    m_settings->setValue(m_keyCtx->startAddress, startAddress());
    updateClientTableView(m_textFormat, ui->comboBoxFormat->currentData().toInt());
}

void ModbusAssistant::onAddressNumberChanged()
{
    m_settings->setValue(m_keyCtx->addressNumber, ui->spinBoxQuantity->value());
    updateClientTableView(m_textFormat, ui->comboBoxFormat->currentData().toInt());
}

void ModbusAssistant::onReadClicked()
{
    if (!isConnected()) {
        return;
    }

    if (!ModbusFactory::Instance()->isClientDevice(m_modbusDevice)) {
        return;
    }

    int registerType = ui->comboBoxFunctionCode->currentData().toInt();
    quint16 start_address = startAddress();
    quint16 quantity = ui->spinBoxQuantity->value();
    quint16 spinBoxServerAddress = ui->spinBoxDeviceAddress->value();
    quint8 function_code = getClientFunctionCode();

    qInfo() << "[SendReadRequest]"
            << "register type:" << registerType << "start address:" << start_address
            << "quantity:" << quantity << "server address:" << spinBoxServerAddress;

    typedef QModbusDataUnit::RegisterType RegisterType;
    RegisterType type = static_cast<RegisterType>(registerType);
    QModbusDataUnit dataUnit(type, start_address, quantity);
    QModbusClient *client = qobject_cast<QModbusClient *>(m_modbusDevice);
    QModbusReply *reply = client->sendReadRequest(dataUnit, spinBoxServerAddress);
    if (!ModbusFactory::Instance()->isValidModbusReply(reply)) {
        return;
    }

    QString info = ui->comboBoxFunctionCode->currentText();
    outputMessage(info, false, TXCOLOR, TXFLAG);
    connect(reply, &QModbusReply::finished, this, [=]() {
        outputModbusReply(reply, function_code);

        if (reply->error() == QModbusDevice::NoError) {
            updateClientTableViewData(reply->result().values());
            reply->deleteLater();
        }
    });
}

void ModbusAssistant::onWriteClicked()
{
    if (!isConnected()) {
        return;
    }

    int registerType = ui->comboBoxFunctionCode->currentData().toInt();
    int start_address = startAddress();
    int spinBoxServerAddress = ui->spinBoxDeviceAddress->value();
    quint8 function_code = getClientFunctionCode();
    QList<quint16> values = getClientRegisterValue();
    ModbusFactory *factory = ModbusFactory::Instance();
    QModbusReply *reply = factory->sendWriteRequest(m_modbusDevice,
                                                    registerType,
                                                    start_address,
                                                    values,
                                                    spinBoxServerAddress);
    if (ModbusFactory::Instance()->isValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=]() {
            outputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = ui->comboBoxFunctionCode->currentText();
        outputMessage(info, false, TXCOLOR, TXFLAG);
    }
}

void ModbusAssistant::onSendClicked()
{
    if (!isConnected()) {
        return;
    }

    quint8 spinBoxServerAddress = ui->spinBoxDeviceAddress->value();
    QByteArray pdu = getClientPdu();
    QByteArray data = pdu;
    if (!data.isEmpty()) {
        data = data.remove(0, 1);
    }

    int function_code = int(QModbusDataUnit::Invalid);
    if (!pdu.isEmpty()) {
        function_code = pdu.at(0);
    }
    ModbusFactory *factory = ModbusFactory::Instance();
    QModbusReply *reply = factory->sendRawRequest(m_modbusDevice,
                                                  spinBoxServerAddress,
                                                  function_code,
                                                  data);

    qWarning() << "Send raw request:"
               << "server address:" << spinBoxServerAddress << "function code:" << function_code
               << "data:" << QString::fromLatin1(pdu.toHex(' '));
    if (ModbusFactory::Instance()->isValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=]() {
            outputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = "pdu(No server address, no crc):";
        info += QString::fromLatin1(pdu.toHex(' '));
        outputMessage(info, false, TXCOLOR, TXFLAG);
    }
}

void ModbusAssistant::onDateWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    qInfo() << "tx:" << "table:" << table << "start:" << address << "size:" << size;
    QTableView *tv = getTableView(table);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer *>(m_modbusDevice);
    QList<quint16> data = ModbusFactory::Instance()->getServerData(server, table, address, size);
    size = qMin<int>(data.count(), size);
    for (int i = 0; i < size; i++) {
        int row = address + i;
        int base = m_textFormat == static_cast<int>(xTools::xIO::TextFormat::Hex) ? 16 : 10;
        int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
        int value = data.at(i);
        QString cookedStr = QString::number(value, base);
        QStandardItem *item = model->item(row, 1);
        if (item) {
            if (base == 16) {
                item->setText(cookedStr.toUpper().rightJustified(width, '0'));
            } else {
                item->setText(cookedStr);
            }
        }
    }

    tv->viewport()->update();
}

void ModbusAssistant::onItemChanged(QStandardItem *item)
{
    if (!item) {
        return;
    }

    if (item->column() != 1) {
        return;
    }

    if (ModbusFactory::Instance()->isServerDevice(m_modbusDevice)) {
        int address = item->row();
        int current_index = ui->tabWidgetServerRegisters->currentIndex();
        QModbusDataUnit::RegisterType table = QModbusDataUnit::Invalid;
        if (current_index == 0) {
            table = QModbusDataUnit::Coils;
        } else if (current_index == 1) {
            table = QModbusDataUnit::DiscreteInputs;
        } else if (current_index == 2) {
            table = QModbusDataUnit::InputRegisters;
        } else if (current_index == 3) {
            table = QModbusDataUnit::HoldingRegisters;
        } else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow table type!");
            return;
        }

        quint16 value = item->text().toInt(Q_NULLPTR, 16);
        ModbusFactory::Instance()->setServerData(m_modbusDevice, table, address, value);
    }
}

void ModbusAssistant::onInputFormatChanged()
{
    int currentFormat = ui->comboBoxFormat->currentData().toInt();
    if (currentFormat == m_textFormat) {
        return;
    }

    QString str = ui->lineEditStartAddress->text().trimmed();
    int value;
    if (m_textFormat == static_cast<int>(xTools::xIO::TextFormat::Hex)) {
        value = str.toInt(nullptr, 16);
    } else {
        value = str.toInt(nullptr, 10);
    }

    if (currentFormat == static_cast<int>(xTools::xIO::TextFormat::Hex)) {
        ui->lineEditStartAddress->setText(QString("%1").arg(QString::number(value, 16), 4, '0'));
    } else {
        ui->lineEditStartAddress->setText(QString::number(value, 10));
    }

    updateClientTableView(m_textFormat, ui->comboBoxFormat->currentData().toInt());
    updateServerRegistersViews(m_textFormat, ui->comboBoxFormat->currentData().toInt());

    m_textFormat = currentFormat;
}

QModbusDevice *ModbusAssistant::createModbusDevice()
{
    QModbusDevice *device = Q_NULLPTR;
    int type = ui->comboBoxDeviceList->currentData().toInt();
    if (ModbusFactory::Instance()->isRtuSerialDeviceType(type)) {
        QString port_name = ui->comboBoxPortName->currentText();
        int parity = ui->comboBoxParity->currentData().toInt();
        int baud_rate = ui->comboBoxBaudRate->currentData().toInt();
        int data_bits = ui->comboBoxDataBits->currentData().toInt();
        int stop_bits = ui->comboBoxStopBits->currentData().toInt();
        ModbusFactory *factory = ModbusFactory::Instance();
        device = factory->createRtuSerialDevice(type,
                                                port_name,
                                                parity,
                                                baud_rate,
                                                data_bits,
                                                stop_bits);
    } else if (ModbusFactory::Instance()->isTcpDeviceType(type)) {
        QString address = ui->comboBoxAddress->currentText();
        int port = ui->spinBoxPort->value();
        ModbusFactory *factory = ModbusFactory::Instance();
        device = factory->createTcpDevice(type, address, port);
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown device type");
    }

    return device;
}

QTableView *ModbusAssistant::createTableView(int rowCount, QTableView *tableView)
{
    if (!tableView) {
        tableView = new QTableView(this);
    }

    QHeaderView *hv = tableView->horizontalHeader();
    QStandardItemModel *model = new QStandardItemModel(tableView);
    QStringList labels = QStringList() << tr("Address") << tr("Value") << tr("Description");
    model->setHorizontalHeaderLabels(labels);
    model->setColumnCount(3);
    model->setRowCount(rowCount);
    tableView->setModel(model);
    tableView->verticalHeader()->hide();
    tableView->setItemDelegateForColumn(0, new ReadOnlyDelegate(tableView));
    updateClientTableViewAddress(tableView, 0);
    hv->setStretchLastSection(true);

    // Set the default value to 0.
    model->blockSignals(true);
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = model->index(row, 1);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, "0000");
        model->setItemData(index, roles);

        QStandardItem *item = model->item(row, 1);
        item->setTextAlignment(Qt::AlignCenter);
    }
    model->blockSignals(false);

    connect(model, &QStandardItemModel::itemChanged, this, &ModbusAssistant::onItemChanged);

    return tableView;
}

void ModbusAssistant::updateUiState(bool connected)
{
    ui->comboBoxDeviceList->setEnabled(!connected);
    ui->pushButtonClose->setEnabled(connected);
    ui->pushButtonOpen->setEnabled(!connected);
    ui->widgetNetworkSettings->setEnabled(!connected);
    ui->widgetSerialPortSettings->setEnabled(!connected);
#if 0
    ui->clientGroupBox->setEnabled(!connected);
    ui->widgetServerParameters->setEnabled(!connected);
#endif
}

void ModbusAssistant::updateClientTableView(int currentFormat, int targetFormat)
{
    int number = ui->spinBoxQuantity->value();
    int rowCount = m_clientRegisterModel->rowCount();
    if (number > rowCount) {
        m_clientRegisterModel->insertRows(rowCount, number - rowCount);
        for (int i = rowCount; i < number; i++) {
            m_clientRegisterModel->setData(m_clientRegisterModel->index(i, 0), i);
        }
    } else if (number < rowCount) {
        m_clientRegisterModel->removeRows(number, rowCount - number);
    }

    for (int i = 0; i < rowCount; i++) {
        for (int col = 0; col < 2; col++) {
            QModelIndex index = m_clientRegisterModel->index(i, col);
            auto *item = m_clientRegisterModel->itemFromIndex(index);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

    // Update address.
    int startAddress = this->startAddress();
    updateClientTableViewAddress(ui->tabViewClientRegisters, startAddress);
    updateClientTableViewData(currentFormat, targetFormat);
}

void ModbusAssistant::updateClientTableViewData(int currentFormat, int targetFormat)
{
    int row = m_clientRegisterModel->rowCount();
    QList<quint16> values;
    for (int i = 0; i < row; i++) {
        auto item = m_clientRegisterModel->item(i, 1);
        if (!item) {
            continue;
        }

        QString str = item->text();
        if (currentFormat == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
            values.append(str.toInt());
        } else {
            values.append(str.toInt(Q_NULLPTR, 16));
        }
    }

    updateClientTableViewData(values);
}

void ModbusAssistant::updateClientTableViewData(const QList<quint16> &values)
{
    for (int row = 0; row < values.count(); row++) {
        auto *item = m_clientRegisterModel->item(row, 1);
        if (!item) {
            continue;
        }

        item->setTextAlignment(Qt::AlignCenter);
        QString text = item->text();
        int value = 0;
        if (m_textFormat == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
            value = text.toInt();
        } else {
            value = text.toInt(Q_NULLPTR, 16);
        }

        int format = ui->comboBoxFormat->currentData().toInt();
        if (format == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
            item->setText(QString::number(value));
        } else {
            item->setText(QString("%1").arg(QString::number(value, 16), 4, '0').toUpper());
        }
    }

    // Refresh the view, or the new value will not be show.
    ui->tabViewClientRegisters->viewport()->update();
}

void ModbusAssistant::updateClientTableViewAddress(QTableView *view, int startAddress)
{
    auto *model = qobject_cast<QStandardItemModel *>(view->model());
    for (int row = 0; row < model->rowCount(); row++) {
        int address = row + startAddress;
        int format = ui->comboBoxFormat->currentData().toInt();
        QString text;
        if (format == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
            text = QString("%1").arg(QString::number(address), 5, '0');
        } else {
            text = QString("%1").arg(QString::number(address, 16), 4, '0').toUpper();
        }

        QModelIndex index = model->index(row, 0);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, text);
        roles.insert(Qt::UserRole, address);
        model->setItemData(index, roles);

        QStandardItem *item = model->item(row, 0);
        if (item) {
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    ui->tabViewClientRegisters->viewport()->update();
}

void ModbusAssistant::updateClientReadWriteButtonState()
{
    QStringList list = ui->comboBoxFunctionCode->currentText().split('-');
    int code = list.length() ? list.first().toInt(Q_NULLPTR, 16) : 0;
    bool is_reading_operation = false;
    if (code == 0x01 || code == 0x02 || code == 0x03 || code == 0x04) {
        is_reading_operation = true;
    }

    if (code == 0x05 || code == 0x06) {
        ui->spinBoxQuantity->setValue(1);
    }

    ui->pushButtonRead->setEnabled(is_reading_operation);
    ui->pushButtonWrite->setEnabled(!is_reading_operation);
}

void ModbusAssistant::updateClientParameters()
{
    int timeout = ui->spinBoxTimeout->value();
    int repeat_time = ui->spinBoxRepeatTime->value();
    ModbusFactory::Instance()->setClientDeviceParameters(m_modbusDevice, timeout, repeat_time);
}

void ModbusAssistant::updateServerParameters()
{
    bool device_busy = ui->checkBoxDeviceBusy->isChecked();
    bool listen_only_mode = ui->checkBoxListenOnlyMode->isChecked();
    int address = ui->spinBoxServerAddress->value();
    ModbusFactory::Instance()->setServerDeviceParameters(m_modbusDevice,
                                                         address,
                                                         device_busy,
                                                         listen_only_mode);
}

bool ModbusAssistant::updateServerMap(QModbusDevice *server)
{
    if (server && qobject_cast<QModbusServer *>(server)) {
        QVector<quint16> values(UINT16_MAX + 1, 0);
        QModbusDataUnit dataUnit(QModbusDataUnit::Coils, 0, values);

        QModbusDataUnitMap map;
        map.insert(QModbusDataUnit::Coils, dataUnit);
        map.insert(QModbusDataUnit::DiscreteInputs, dataUnit);
        map.insert(QModbusDataUnit::HoldingRegisters, dataUnit);
        map.insert(QModbusDataUnit::InputRegisters, dataUnit);

        QModbusServer *cooked_server = qobject_cast<QModbusServer *>(server);
        cooked_server->blockSignals(true);
        bool is_ok = cooked_server->setMap(map);
        cooked_server->blockSignals(false);
        return is_ok;
    }

    return false;
}

void ModbusAssistant::updateServerRegistersViews(int currentFormat, int targetFormat)
{
    for (int i = 0; i < 4; i++) {
        QWidget *widget = ui->tabWidgetServerRegisters->widget(i);
        QTableView *tableView = qobject_cast<QTableView *>(widget);
        auto *model = qobject_cast<QStandardItemModel *>(tableView->model());
        int type = QModbusDataUnit::Invalid;
        if (i == 0) {
            type = QModbusDataUnit::Coils;
        } else if (i == 1) {
            type = QModbusDataUnit::DiscreteInputs;
        } else if (i == 2) {
            type = QModbusDataUnit::InputRegisters;
        } else if (i == 3) {
            type = QModbusDataUnit::HoldingRegisters;
        } else {
            qWarning() << "Unknown register type.";
            continue;
        }

        for (int row = 0; row < model->rowCount(); row++) {
            QStandardItem *item = model->item(row, 1);
            if (!item) {
                continue;
            }

            quint16 value;
            int hex = static_cast<int>(xTools::xIO::TextFormat::Hex);
            if (currentFormat == hex) {
                value = item ? item->text().toInt(Q_NULLPTR, 16) : 0;
            } else {
                value = item ? item->text().toInt(Q_NULLPTR, 10) : 0;
            }

            if (targetFormat == hex) {
                item->setText(QString("%1").arg(QString::number(value, 16), 4, '0').toUpper());
            } else {
                item->setText(QString("%1").arg(QString::number(value), 5, '0'));
            }

            auto table = static_cast<QModbusDataUnit::RegisterType>(type);
            ModbusFactory::Instance()->setServerData(m_modbusDevice, table, row, value, false);
        }

        tableView->viewport()->update();
    }
}

quint8 ModbusAssistant::getClientFunctionCode()
{
    QString txt = ui->comboBoxFunctionCode->currentText();
    QStringList list = txt.split('-', Qt::SkipEmptyParts);
    if (list.length()) {
        return list.first().toInt(Q_NULLPTR, 16);
    }

    return 0;
}

QList<quint16> ModbusAssistant::getClientRegisterValue()
{
    QList<quint16> values;
    for (int row = 0; row < m_clientRegisterModel->rowCount(); row++) {
        QStandardItem *item = m_clientRegisterModel->item(row, 1);
        if (item) {
            QString text = item->text();
            if (m_textFormat == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
                values.append(text.toInt());
            } else {
                values.append(text.toInt(Q_NULLPTR, 16));
            }
        } else {
            values.append(0);
        }
    }

    return values;
}

QByteArray ModbusAssistant::getClientPdu()
{
    QString text = ui->lineEditPdu->text();
    QStringList valueList = text.split(' ', Qt::SkipEmptyParts);
    QByteArray data;
    for (QString &value : valueList) {
        data.append(char(value.toInt(Q_NULLPTR, 16)));
    }

    return data;
}

QTableView *ModbusAssistant::getTableView(QModbusDataUnit::RegisterType table)
{
    QWidget *tv = Q_NULLPTR;
    if (table == QModbusDataUnit::Coils) {
        tv = ui->tabWidgetServerRegisters->widget(0);
    } else if (table == QModbusDataUnit::DiscreteInputs) {
        tv = ui->tabWidgetServerRegisters->widget(1);
    } else if (table == QModbusDataUnit::InputRegisters) {
        tv = ui->tabWidgetServerRegisters->widget(2);
    } else if (table == QModbusDataUnit::HoldingRegisters) {
        tv = ui->tabWidgetServerRegisters->widget(3);
    } else {
        tv = ui->tabWidgetServerRegisters->widget(3);
        qWarning() << "Get table view failed: unknow register type!";
    }

    return qobject_cast<QTableView *>(tv);
}

QList<quint16> ModbusAssistant::getTableValues(QTableView *tableView, int row, int count)
{
    if (!tableView) {
        qWarning() << "Table view can not be null!";
        return QList<quint16>();
    }

    auto *model = qobject_cast<QStandardItemModel *>(tableView->model());
    if (!model) {
        qWarning() << "Model can not be null!";
        return QList<quint16>();
    }

    QList<quint16> values;
    for (int i = row; i < count; i++) {
        QStandardItem *item = model->item(i, 1);
        if (item) {
            QString text = item->text();
            values.append(text.toInt(Q_NULLPTR, 16));
        } else {
            values.append(0);
        }
    }

    return values;
}

void ModbusAssistant::outputModbusReply(QModbusReply *reply, int functionCode)
{
    if (reply->error() != QModbusDevice::NoError) {
        outputMessage(reply->errorString(), true, "", "error");
        QMessageBox::warning(this, tr("Error occured"), reply->errorString());
        return;
    }

    int spinBoxServerAddress = reply->serverAddress();
    if (reply->type() == QModbusReply::ReplyType::Raw) {
        QByteArray data = reply->rawResult().data();
        QString info = QString("(from address: %1, "
                               "function code: %2), "
                               "data unit: %3")
                           .arg(spinBoxServerAddress)
                           .arg(functionCode)
                           .arg(QString::fromLatin1(data.toHex(' ')));
        outputMessage(info, false, RXCOLOR, RXFLAG);
    } else if (reply->type() == QModbusReply::ReplyType::Common) {
        QString info = ui->comboBoxFunctionCode->currentText();
        outputMessage(info, false, RXCOLOR, RXFLAG);
    }
}

void ModbusAssistant::outputMessage(const QString &msg,
                                    bool isError,
                                    const QString &color,
                                    const QString &flag)
{
    QString cookedMsg = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    cookedMsg = QString("<font color=silver>%1 </font>").arg(cookedMsg);

    QString cookedColor = color;
    if (cookedColor.isEmpty()) {
        if (isError) {
            cookedColor = "red";
        } else {
            cookedColor = "black";
        }
    }

    cookedMsg += flag.isEmpty() ? ""
                                : QString("<font color=%1>[%2]</font> ").arg(cookedColor, flag);
    cookedMsg += msg;
    ui->textBrowser->append(cookedMsg);
}

bool ModbusAssistant::isConnected()
{
    if (ModbusFactory::Instance()->isConnected(m_modbusDevice)) {
        return true;
    }

    QMessageBox::warning(this,
                         tr("Device is not Ready"),
                         tr("The modbus device is not ready, "
                            "please check settings and try again later!"));
    return false;
}

bool ModbusAssistant::writeSettingsArray(
    const QString &group, const QString &key, const QString &value, int index, int maxIndex)
{
    m_settings->beginWriteArray(group);
    for (int i = 0; i < maxIndex; i++) {
        m_settings->setArrayIndex(i);
        QString v = m_settings->value(key).toString();
        if (v == value) {
            m_settings->endArray();
            return false;
        }
    }

    m_settings->setArrayIndex(index);
    m_settings->setValue(key, value);
    m_settings->endArray();
    return true;
}

int ModbusAssistant::startAddress()
{
    QString text = ui->lineEditStartAddress->text();
    int format = ui->comboBoxFormat->currentData().toInt();
    if (format == static_cast<int>(xTools::xIO::TextFormat::Dec)) {
        return text.toInt();
    } else if (format == static_cast<int>(xTools::xIO::TextFormat::Hex)) {
        return text.toInt(Q_NULLPTR, 16);
    }

    Q_ASSERT_X(false, __FUNCTION__, "Unknown format.");
    return 0;
}
