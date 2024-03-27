/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsModbusStudioUi.h"
#include "ui_xToolsModbusStudioUi.h"

#include <QAbstractSocket>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QGroupBox>
#include <QHostAddress>
#include <QItemDelegate>
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
#include <QTextBrowser>
#include <QtEndian>
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
#include <QModbusRtuSerialMaster>
#include <QModbusRtuSerialSlave>
#else
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#endif

#include "xToolsCompatibility.h"
#include "xToolsModbusStudio.h"
#include "xToolsSettings.h"

#define RXCOLOR "green"
#define TXCOLOR "blue"
#define RXFLAG "rx:"
#define TXFLAG "tx:"
#define MAX_HISTORY_INDEX 9

struct xToolsModbusUiSettingKeys
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

    const QString sendHistory = "xToolsModbus/sendHistory";
    const QString sendHistoryIndex = "xToolsModbus/sendHistoryIndex";
    const QString pdu = "pdu";
};

class ReadOnlyDelegate : public QItemDelegate
{
public:
    ReadOnlyDelegate(QWidget *parent = Q_NULLPTR)
        : QItemDelegate(parent)
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

xToolsModbusStudioUi::xToolsModbusStudioUi(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::xToolsModbusStudioUi)
    , m_modbusDevice(Q_NULLPTR)
    , m_registerModel(Q_NULLPTR)
    , m_keyCtx(new xToolsModbusUiSettingKeys)
{
    if (!m_settings) {
        m_settings = xToolsSettings::instance();
    }

    ui->setupUi(this);

    initComponents();
    initSettings();
    initSignals();

    updateUiState(false);
    onDeviceTypeChanged();
    updateClientTableView();
    updateClientReadWriteButtonState();
}

xToolsModbusStudioUi::~xToolsModbusStudioUi()
{
    delete ui;
}

void xToolsModbusStudioUi::initComponents()
{
    initComponentDevices();
    initComponentAddress();
    initComponentPortName();
    initComponnetBaudRate();
    initComponnetDataBits();
    initComponnetStopBits();
    initComponnetParity();
    initComponentFunctionCode();
    initComponentRegisterTableView();
    initComponentInput();
    initComponentRegisterTabWidget();
}

void xToolsModbusStudioUi::initComponentDevices()
{
    ui->device_list_->addItem(tr("RtuClient"), xToolsModbusStudio::kModbusRtuSerialClient);
    ui->device_list_->addItem(tr("RtuServer"), xToolsModbusStudio::kModbusRtuSerialServer);
    ui->device_list_->addItem(tr("TcpClient"), xToolsModbusStudio::kModbusTcpClient);
    ui->device_list_->addItem(tr("TcpServer"), xToolsModbusStudio::kModbusTcpServer);
}

void xToolsModbusStudioUi::initComponentAddress()
{
    ui->address_combo_box->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->address_combo_box->addItem(address.toString());
        }
    }
}

void xToolsModbusStudioUi::initComponentPortName()
{
    ui->port_name_->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui->port_name_->addItem(info.portName());
    }
}

void xToolsModbusStudioUi::initComponnetBaudRate()
{
    ui->baud_rate_->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui->baud_rate_->addItem(QString::number(bd), bd);
    }
}

void xToolsModbusStudioUi::initComponnetDataBits()
{
    ui->data_bits_->clear();
    ui->data_bits_->addItem("8", QSerialPort::Data8);
    ui->data_bits_->addItem("7", QSerialPort::Data7);
    ui->data_bits_->addItem("6", QSerialPort::Data6);
    ui->data_bits_->addItem("5", QSerialPort::Data5);
}

void xToolsModbusStudioUi::initComponnetStopBits()
{
    ui->stop_bits_->clear();
    ui->stop_bits_->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stop_bits_->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->stop_bits_->addItem("2", QSerialPort::TwoStop);
}

void xToolsModbusStudioUi::initComponnetParity()
{
    ui->parity_->clear();
    ui->parity_->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui->parity_->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui->parity_->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui->parity_->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui->parity_->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void xToolsModbusStudioUi::initComponentFunctionCode()
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

    ui->function_code_->addItem(str0x01, func0x01);
    ui->function_code_->addItem(str0x02, func0x02);
    ui->function_code_->addItem(str0x03, func0x03);
    ui->function_code_->addItem(str0x04, func0x04);
    ui->function_code_->addItem(str0x05, func0x05);
    ui->function_code_->addItem(str0x06, func0x06);
    ui->function_code_->addItem(str0x0f, func0x0f);
    ui->function_code_->addItem(str0x10, func0x10);
}

void xToolsModbusStudioUi::initComponentRegisterTableView()
{
    QTableView *table_view = CreateTableView(1, ui->client_registers_);
    m_registerModel = qobject_cast<QStandardItemModel *>(table_view->model());
}

void xToolsModbusStudioUi::initComponentInput()
{
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui->pdu_->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void xToolsModbusStudioUi::initComponentRegisterTabWidget()
{
    QTabWidget *tab_widget = ui->server_registers_;
    QStringList titles = QStringList() << tr("Coils") << tr("DiscreteInputs")
                                       << tr("InputRegisters") << tr("HoldingRegisters");
    for (QString &title : titles) {
        QTableView *table_view = CreateTableView(UINT16_MAX + 1, Q_NULLPTR);
        table_view->verticalHeader()->hide();
        tab_widget->addTab(table_view, title);
    }
}

void xToolsModbusStudioUi::initSettings()
{
    initSettingsDevice();
    initSettingsNetwork();
    initSettingsSerialPort();
    initSettingsClient();
    initSettingsServer();
    initSettingsClientOperations();
    initSettingsInput();
}

void xToolsModbusStudioUi::initSettingsDevice()
{
    int deviceIndex = m_settings->value(m_keyCtx->deviceIndex).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui->device_list_->count()) {
        ui->device_list_->setCurrentIndex(deviceIndex);
    }
}

void xToolsModbusStudioUi::initSettingsNetwork()
{
    QString address = m_settings->value(m_keyCtx->address).toString();
    ui->address_combo_box->setCurrentText(address);

    QVariant portValiant = m_settings->value(m_keyCtx->port);
    int port = portValiant.toInt();
    if (portValiant.isNull()) {
        port = 502;
    }
    ui->port_spin_box->setValue(port);
}

void xToolsModbusStudioUi::initSettingsSerialPort()
{
    auto SetComboBoxIndex = [](int index, QComboBox *cb) {
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    };

    QString portName = m_settings->value(m_keyCtx->portName).toString();
    int index = ui->port_name_->findText(portName);
    SetComboBoxIndex(index, ui->port_name_);

    index = m_settings->value(m_keyCtx->parity).toInt();
    SetComboBoxIndex(index, ui->parity_);

    QString bd = m_settings->value(m_keyCtx->baudRate).toString();
    ui->baud_rate_->setCurrentText(bd);

    index = m_settings->value(m_keyCtx->dataBits).toInt();
    SetComboBoxIndex(index, ui->data_bits_);

    index = m_settings->value(m_keyCtx->stopBits).toInt();
    SetComboBoxIndex(index, ui->stop_bits_);
}

void xToolsModbusStudioUi::initSettingsClient()
{
    int timeout = m_settings->value(m_keyCtx->clientTimeout).toInt();
    ui->timeout_->setValue(timeout < 100 ? 100 : timeout);

    const QString key = m_keyCtx->clientRepeatTime;
    int repeatTimes = m_settings->value(key).toInt();
    ui->repeat_time_->setValue(repeatTimes);
}

void xToolsModbusStudioUi::initSettingsServer()
{
    bool isBusy = m_settings->value(m_keyCtx->serverIsBusy).toBool();
    ui->device_busy_->setChecked(isBusy);

    QString key = m_keyCtx->serverJustListen;
    bool just_listen = m_settings->value(key).toBool();
    ui->listen_only_mode_->setChecked(just_listen);

    int address = m_settings->value(m_keyCtx->serverAddress).toInt();
    ui->server_address->setValue(address);
}

void xToolsModbusStudioUi::initSettingsClientOperations()
{
    int index = m_settings->value(m_keyCtx->functionCode).toInt();
    if (index >= 0 && index < ui->function_code_->count()) {
        ui->function_code_->setCurrentIndex(index);
    }

    int address = m_settings->value(m_keyCtx->targetAddress).toInt();
    ui->device_address_->setValue(address);

    int start = m_settings->value(m_keyCtx->startAddress).toInt();
    ui->start_address_->setValue(start);

    int number = m_settings->value(m_keyCtx->addressNumber).toInt();
    ui->quantity_->setValue(number);
}

void xToolsModbusStudioUi::initSettingsInput()
{
    ui->pdu_->clear();
    m_settings->beginReadArray(m_keyCtx->sendHistory);
    for (int i = 0; i < 10; i++) {
        m_settings->setArrayIndex(i);
        QString text = m_settings->value(m_keyCtx->pdu).toString();
        if (!text.isEmpty()) {
            ui->pdu_->addItem(text);
        }
    }
    m_settings->endArray();

    int index = m_settings->value(m_keyCtx->sendHistoryIndex).toInt();
    ui->pdu_->setCurrentIndex(index - 1);
}

void xToolsModbusStudioUi::initSignals()
{
    initSignalsDevice();
    initSignalsNetworking();
    initSignalsSerialPort();
    initSignalsClient();
    initSignalsServer();
    initSignalsClientOperations();
}

void xToolsModbusStudioUi::initSignalsDevice()
{
    connect(ui->device_list_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::onDeviceTypeChanged);
    connect(ui->open_button_, &QPushButton::clicked, this, &xToolsModbusStudioUi::onOpenClicked);
    connect(ui->cloese_button_, &QPushButton::clicked, this, &xToolsModbusStudioUi::onCloseClicked);
}

void xToolsModbusStudioUi::initSignalsNetworking()
{
    connect(ui->address_combo_box,
            &QComboBox::currentTextChanged,
            this,
            &xToolsModbusStudioUi::onAddressChanged);
    connect(ui->port_spin_box,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onPortChanged);
}

void xToolsModbusStudioUi::initSignalsSerialPort()
{
    connect(ui->port_name_,
            &QComboBox::currentTextChanged,
            this,
            &xToolsModbusStudioUi::onPortNameChanged);
    connect(ui->parity_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::onParityChanged);
    connect(ui->baud_rate_,
            &QComboBox::currentTextChanged,
            this,
            &xToolsModbusStudioUi::onBaudRateChanged);
    connect(ui->data_bits_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::onDataBitsChanged);
    connect(ui->stop_bits_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::onStopBistChanged);
}

void xToolsModbusStudioUi::initSignalsClient()
{
    connect(ui->timeout_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onClientTimeoutChanged);
    connect(ui->repeat_time_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onClientRepeatTimeChanged);
}

void xToolsModbusStudioUi::initSignalsServer()
{
    connect(ui->device_busy_,
            &QCheckBox::clicked,
            this,
            &xToolsModbusStudioUi::onServerIsBusyChanged);
    connect(ui->listen_only_mode_,
            &QCheckBox::clicked,
            this,
            &xToolsModbusStudioUi::onServerJustListenChanged);
    connect(ui->server_address,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onServerAddressChanged);
}

void xToolsModbusStudioUi::initSignalsClientOperations()
{
    connect(ui->function_code_,
            &QComboBox::currentTextChanged,
            this,
            &xToolsModbusStudioUi::onFunctionCodeChanged);
    connect(ui->device_address_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onTargetAddressChanged);
    connect(ui->start_address_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onStartAddressChanged);
    connect(ui->quantity_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::onAddressNumberChanged);
    connect(ui->read_, &QPushButton::clicked, this, &xToolsModbusStudioUi::onReadClicked);
    connect(ui->write_, &QPushButton::clicked, this, &xToolsModbusStudioUi::onWriteClicked);
    connect(ui->send_, &QPushButton::clicked, this, &xToolsModbusStudioUi::onSendClicked);
}

void xToolsModbusStudioUi::onErrorOccurred()
{
    outputMessage(m_modbusDevice->errorString(), true, "", "error");
    if (m_modbusDevice->error() == QModbusDevice::ConnectionError) {
        const QString msg = m_modbusDevice->errorString();
        onCloseClicked();
        QMessageBox::warning(this, tr("Error occured"), msg);
    }
}

void xToolsModbusStudioUi::onDeviceTypeChanged()
{
    int type = ui->device_list_->currentData().toInt();
    bool isSerial = (type == xToolsModbusStudio::kModbusRtuSerialClient
                     || type == xToolsModbusStudio::kModbusRtuSerialServer);
    bool isClient = (type == xToolsModbusStudio::kModbusRtuSerialClient
                     || type == xToolsModbusStudio::kModbusTcpClient);

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
        ui->console_->setVisible(true);
        ui->widgetServerParameters->setHidden(true);
        ui->registersGroupBox->setHidden(true);
        ui->widgetClientParameters->setHidden(false);
        ui->clientCustomCommandGroupBox->setHidden(false);
        ui->clientRegistersGroupBox->setHidden(false);
    } else {
        ui->console_->setVisible(false);
        ui->clientCustomCommandGroupBox->setHidden(true);
        ui->clientRegistersGroupBox->setHidden(true);
        ui->widgetClientParameters->setHidden(true);
        ui->widgetServerParameters->setHidden(false);
        ui->registersGroupBox->setHidden(false);
    }

    m_settings->setValue(m_keyCtx->deviceIndex, type);
}

void xToolsModbusStudioUi::onCloseClicked()
{
    xToolsModbusStudio::Instance()->DeleteModbusDevuce(&m_modbusDevice);
    updateUiState(false);
}

void xToolsModbusStudioUi::onOpenClicked()
{
    ui->open_button_->setEnabled(false);
    xToolsModbusStudio::Instance()->DeleteModbusDevuce(&m_modbusDevice);

    m_modbusDevice = CreateModbusDevice();

    if (xToolsModbusStudio::Instance()->IsServerDevice(m_modbusDevice)) {
        if (!updateServerMap(m_modbusDevice)) {
            ui->open_button_->setEnabled(true);
            qWarning() << "Can not reset server map!";
            return;
        }

        updateServerParameters();

        QModbusServer *server = qobject_cast<QModbusServer *>(m_modbusDevice);
        updateServerRegistersData();
        connect(server, &QModbusServer::dataWritten, this, &xToolsModbusStudioUi::onDateWritten);
    } else if (xToolsModbusStudio::Instance()->IsClientDevice(m_modbusDevice)) {
        updateClientParameters();
    } else {
        ui->open_button_->setEnabled(true);
        qWarning() << "Can not create modbus devices!";
        return;
    }

    connect(m_modbusDevice,
            &QModbusDevice::errorOccurred,
            this,
            &xToolsModbusStudioUi::onErrorOccurred);
    xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
    bool connected = factory->ConnectDeivce(m_modbusDevice);
    if (!connected) {
        QString errStr = m_modbusDevice->errorString();
        QString info = tr("Can not open device: %1."
                          "Please check the parameters and try again!")
                           .arg(errStr);
        QMessageBox::warning(this, tr("Can not open device"), info);
    }

    updateUiState(connected);
}

void xToolsModbusStudioUi::onAddressChanged()
{
    m_settings->setValue(m_keyCtx->address, ui->address_combo_box->currentText());
}

void xToolsModbusStudioUi::onPortChanged()
{
    m_settings->setValue(m_keyCtx->port, ui->port_spin_box->value());
}

void xToolsModbusStudioUi::onPortNameChanged()
{
    m_settings->setValue(m_keyCtx->portName, ui->port_name_->currentText());
}

void xToolsModbusStudioUi::onParityChanged()
{
    m_settings->setValue(m_keyCtx->parity, ui->parity_->currentIndex());
}

void xToolsModbusStudioUi::onBaudRateChanged()
{
    m_settings->setValue(m_keyCtx->baudRate, ui->baud_rate_->currentText());
}

void xToolsModbusStudioUi::onDataBitsChanged()
{
    m_settings->setValue(m_keyCtx->dataBits, ui->data_bits_->currentIndex());
}

void xToolsModbusStudioUi::onStopBistChanged()
{
    m_settings->setValue(m_keyCtx->stopBits, ui->stop_bits_->currentIndex());
}

void xToolsModbusStudioUi::onInvokeRefresh()
{
    initComponentPortName();
}

void xToolsModbusStudioUi::onClientTimeoutChanged()
{
    m_settings->setValue(m_keyCtx->clientTimeout, ui->timeout_->value());
    updateClientParameters();
}

void xToolsModbusStudioUi::onClientRepeatTimeChanged()
{
    m_settings->setValue(m_keyCtx->clientRepeatTime, ui->repeat_time_->value());
    updateClientParameters();
}

void xToolsModbusStudioUi::onServerIsBusyChanged()
{
    m_settings->setValue(m_keyCtx->serverIsBusy, ui->device_busy_->isChecked());
    updateServerParameters();
}

void xToolsModbusStudioUi::onServerJustListenChanged()
{
    m_settings->setValue(m_keyCtx->serverJustListen, ui->listen_only_mode_->isChecked());
    updateServerParameters();
}

void xToolsModbusStudioUi::onServerAddressChanged()
{
    m_settings->setValue(m_keyCtx->serverAddress, ui->server_address->value());
    updateServerParameters();
}

void xToolsModbusStudioUi::onFunctionCodeChanged()
{
    m_settings->setValue(m_keyCtx->functionCode, ui->function_code_->currentIndex());
    updateClientReadWriteButtonState();
}

void xToolsModbusStudioUi::onTargetAddressChanged()
{
    m_settings->setValue(m_keyCtx->targetAddress, ui->device_address_->value());
}

void xToolsModbusStudioUi::onStartAddressChanged()
{
    m_settings->setValue(m_keyCtx->startAddress, ui->start_address_->value());
    updateClientTableView();
}

void xToolsModbusStudioUi::onAddressNumberChanged()
{
    m_settings->setValue(m_keyCtx->addressNumber, ui->quantity_->value());
    updateClientTableView();
}

void xToolsModbusStudioUi::onReadClicked()
{
    if (!isConnected()) {
        return;
    }

    if (!xToolsModbusStudio::Instance()->IsClientDevice(m_modbusDevice)) {
        return;
    }

    int register_type = ui->function_code_->currentData().toInt();
    quint16 start_address = ui->start_address_->value();
    quint16 quantity = ui->quantity_->value();
    quint16 server_address = ui->device_address_->value();
    quint8 function_code = getClientFunctionCode();

    qInfo() << "[SendReadRequest]"
            << "register type:" << register_type << "start address:" << start_address
            << "quantity:" << quantity << "server address:" << server_address;

    typedef QModbusDataUnit::RegisterType RegisterType;
    RegisterType type = static_cast<RegisterType>(register_type);
    QModbusDataUnit data_unit(type, start_address, quantity);
    QModbusClient *client = qobject_cast<QModbusClient *>(m_modbusDevice);
    QModbusReply *reply = client->sendReadRequest(data_unit, server_address);
    if (!xToolsModbusStudio::Instance()->IsValidModbusReply(reply)) {
        return;
    }

    QString info = ui->function_code_->currentText();
    outputMessage(info, false, TXCOLOR, TXFLAG);
    connect(reply, &QModbusReply::finished, this, [=]() {
        outputModbusReply(reply, function_code);

        if (reply->error() == QModbusDevice::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            UpdateClientTableViewData(reply->result().values());
#else
            updateClientTableViewData(vectorTolist(reply->result().values()));
#endif
            reply->deleteLater();
        }
    });
}

void xToolsModbusStudioUi::onWriteClicked()
{
    if (!isConnected()) {
        return;
    }

    int registerType = ui->function_code_->currentData().toInt();
    int start_address = ui->start_address_->value();
    int server_address = ui->device_address_->value();
    quint8 function_code = getClientFunctionCode();
    QList<quint16> values = getClientRegisterValue();
    xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
    QModbusReply *reply = factory->SendWriteRequest(m_modbusDevice,
                                                    registerType,
                                                    start_address,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                    values,
#else
                                                    listToVector(values),
#endif
                                                    server_address);
    if (xToolsModbusStudio::Instance()->IsValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=]() {
            outputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = ui->function_code_->currentText();
        outputMessage(info, false, TXCOLOR, TXFLAG);
    }
}

void xToolsModbusStudioUi::onSendClicked()
{
    if (!isConnected()) {
        return;
    }

    quint8 server_address = ui->device_address_->value();
    QByteArray pdu = getClientPdu();
    QByteArray data = pdu;
    if (!data.isEmpty()) {
        data = data.remove(0, 1);
    }

    int function_code = int(QModbusDataUnit::Invalid);
    if (!pdu.isEmpty()) {
        function_code = pdu.at(0);
    }
    xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
    QModbusReply *reply = factory->SendRawRequest(m_modbusDevice,
                                                  server_address,
                                                  function_code,
                                                  data);

    qWarning() << "Send raw request:"
               << "server address:" << server_address << "function code:" << function_code
               << "data:" << QString(xToolsByteArrayToHex(pdu, ' '));
    if (xToolsModbusStudio::Instance()->IsValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=]() {
            outputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = "pdu(No server address, no crc):";
        info += QString(xToolsByteArrayToHex(pdu, ' '));
        outputMessage(info, false, TXCOLOR, TXFLAG);
    }

    // Update settings data.
    int index = m_settings->value(m_keyCtx->sendHistoryIndex).toInt();
    bool ret = writeSettingsArray(m_keyCtx->sendHistory,
                                  m_keyCtx->pdu,
                                  QString(xToolsByteArrayToHex(pdu, ' ')),
                                  index,
                                  MAX_HISTORY_INDEX);
    if (!ret) {
        return;
    }

    if (index > ui->pdu_->count()) {
        ui->pdu_->addItem(QString(xToolsByteArrayToHex(pdu, ' ')));
    } else {
        ui->pdu_->insertItem(index, QString(xToolsByteArrayToHex(pdu, ' ')));
    }

    index = index + 1 > MAX_HISTORY_INDEX ? 0 : index + 1;
    m_settings->setValue(m_keyCtx->sendHistoryIndex, index);
}

void xToolsModbusStudioUi::onDateWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    qInfo() << "Data written:"
            << "table:" << table << "start address:" << address << "size:" << size;
    QTableView *tv = getTableView(table);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer *>(m_modbusDevice);
    QList<quint16> data = xToolsModbusStudio::Instance()->GetServerData(server, table, address, size);
    size = qMin<int>(data.count(), size);
    for (int i = 0; i < size; i++) {
        int row = address + i;
        int base = 16;
        int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
        int value = data.at(i);
        QString cooked_str = QString::number(value, base);
        cooked_str = cooked_str.rightJustified(width, '0', true);
        QStandardItem *item = model->item(row, 1);
        if (item) {
            item->setData(cooked_str, Qt::DisplayRole);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    tv->viewport()->update();
}

void xToolsModbusStudioUi::onItemChanged(QStandardItem *item)
{
    if (!item) {
        return;
    }

    if (item->column() != 1) {
        return;
    }

    if (xToolsModbusStudio::Instance()->IsServerDevice(m_modbusDevice)) {
        int address = item->row();
        int current_index = ui->server_registers_->currentIndex();
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
        xToolsModbusStudio::Instance()->SetServerData(m_modbusDevice, table, address, value);
    }
}

QModbusDevice *xToolsModbusStudioUi::CreateModbusDevice()
{
    QModbusDevice *device = Q_NULLPTR;
    int type = ui->device_list_->currentData().toInt();
    if (xToolsModbusStudio::Instance()->IsRtuSerialDeviceType(type)) {
        QString port_name = ui->port_name_->currentText();
        int parity = ui->parity_->currentData().toInt();
        int baud_rate = ui->baud_rate_->currentData().toInt();
        int data_bits = ui->data_bits_->currentData().toInt();
        int stop_bits = ui->stop_bits_->currentData().toInt();
        xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
        device = factory->CreateRtuSerialDevice(type,
                                                port_name,
                                                parity,
                                                baud_rate,
                                                data_bits,
                                                stop_bits);
    } else if (xToolsModbusStudio::Instance()->IsTcpDeviceType(type)) {
        QString address = ui->address_combo_box->currentText();
        int port = ui->port_spin_box->value();
        xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
        device = factory->CreateTcpDevice(type, address, port);
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown device type");
    }

    return device;
}

QTableView *xToolsModbusStudioUi::CreateTableView(int rowCount, QTableView *tableView)
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

    connect(model, &QStandardItemModel::itemChanged, this, &xToolsModbusStudioUi::onItemChanged);

    return tableView;
}

void xToolsModbusStudioUi::updateUiState(bool connected)
{
    ui->device_list_->setEnabled(!connected);
    ui->cloese_button_->setEnabled(connected);
    ui->open_button_->setEnabled(!connected);
    ui->widgetNetworkSettings->setEnabled(!connected);
    ui->widgetSerialPortSettings->setEnabled(!connected);
#if 0
    ui->clientGroupBox->setEnabled(!connected);
    ui->widgetServerParameters->setEnabled(!connected);
#endif
}

void xToolsModbusStudioUi::updateClientTableView()
{
    int number = ui->quantity_->value();
    int rowCount = m_registerModel->rowCount();
    if (number > rowCount) {
        m_registerModel->insertRows(rowCount, number - rowCount);
    } else if (number < rowCount) {
        m_registerModel->removeRows(number, rowCount - number);
    }

    // Update address.
    int start_address = ui->start_address_->value();
    updateClientTableViewAddress(ui->client_registers_, start_address);
}

void xToolsModbusStudioUi::updateClientTableViewData(const QList<quint16> &values)
{
    for (int row = 0; row < values.count(); row++) {
        int value = values.at(row);
        QModelIndex index = m_registerModel->index(row, 1);
        QMap<int, QVariant> roles;
        QString str = QString("%1").arg(QString::number(value, 16), 4, '0');
        roles.insert(Qt::DisplayRole, str);
        m_registerModel->setItemData(index, roles);
        QStandardItem *item = m_registerModel->item(row, 1);
        if (item) {
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    // Refresh the view, or the new value will not be show.
    ui->client_registers_->viewport()->update();
}

void xToolsModbusStudioUi::updateClientReadWriteButtonState()
{
    QStringList list = ui->function_code_->currentText().split('-');
    int code = list.length() ? list.first().toInt(Q_NULLPTR, 16) : 0;
    bool is_reading_operation = false;
    if (code == 0x01 || code == 0x02 || code == 0x03 || code == 0x04) {
        is_reading_operation = true;
    }

    if (code == 0x05 || code == 0x06) {
        ui->quantity_->setValue(1);
    }

    ui->read_->setEnabled(is_reading_operation);
    ui->write_->setEnabled(!is_reading_operation);
}

void xToolsModbusStudioUi::updateClientParameters()
{
    int timeout = ui->timeout_->value();
    int repeat_time = ui->repeat_time_->value();
    xToolsModbusStudio::Instance()->SetClientDeviceParameters(m_modbusDevice, timeout, repeat_time);
}

void xToolsModbusStudioUi::updateClientTableViewAddress(QTableView *view, int start_address)
{
    auto *model = qobject_cast<QStandardItemModel *>(view->model());
    for (int row = 0; row < model->rowCount(); row++) {
        int address = row + start_address;
        QString text = QString("%1").arg(QString::number(address), 5, '0');
        QModelIndex index = model->index(row, 0);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, text);
        model->setItemData(index, roles);

        QStandardItem *item = model->item(row, 0);
        if (item) {
            item->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void xToolsModbusStudioUi::updateServerParameters()
{
    bool device_busy = ui->device_busy_->isChecked();
    bool listen_only_mode = ui->listen_only_mode_->isChecked();
    int address = ui->server_address->value();
    xToolsModbusStudio::Instance()->SetServerDeviceParameters(m_modbusDevice,
                                                              address,
                                                              device_busy,
                                                              listen_only_mode);
}

bool xToolsModbusStudioUi::updateServerMap(QModbusDevice *server)
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

void xToolsModbusStudioUi::updateServerRegistersData()
{
    for (int i = 0; i < 4; i++) {
        QWidget *widget = ui->server_registers_->widget(i);
        QTableView *table_view = qobject_cast<QTableView *>(widget);
        auto *model = qobject_cast<QStandardItemModel *>(table_view->model());
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
            quint16 value = item ? item->text().toInt(Q_NULLPTR, 16) : 0;
            auto table = static_cast<QModbusDataUnit::RegisterType>(type);
            xToolsModbusStudio::Instance()->SetServerData(m_modbusDevice, table, row, value, false);
        }
    }
}

quint8 xToolsModbusStudioUi::getClientFunctionCode()
{
    QString txt = ui->function_code_->currentText();
    QStringList list = txt.split('-', xToolsSkipEmptyParts);
    if (list.length()) {
        return list.first().toInt(Q_NULLPTR, 16);
    }

    return 0;
}

QList<quint16> xToolsModbusStudioUi::getClientRegisterValue()
{
    QList<quint16> values;
    for (int row = 0; row < m_registerModel->rowCount(); row++) {
        QStandardItem *item = m_registerModel->item(row, 1);
        if (item) {
            QString text = item->text();
            values.append(text.toInt(Q_NULLPTR, 16));
        } else {
            values.append(0);
        }
    }

    return values;
}

QByteArray xToolsModbusStudioUi::getClientPdu()
{
    QString text = ui->pdu_->currentText();
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QStringList valueList = text.split(' ', QString::SkipEmptyParts);
#else
    QStringList valueList = text.split(' ', Qt::SkipEmptyParts);
#endif
    QByteArray data;
    for (QString &value : valueList) {
        data.append(char(value.toInt(Q_NULLPTR, 16)));
    }

    return data;
}

QTableView *xToolsModbusStudioUi::getTableView(QModbusDataUnit::RegisterType table)
{
    QWidget *tv = Q_NULLPTR;
    if (table == QModbusDataUnit::Coils) {
        tv = ui->server_registers_->widget(0);
    } else if (table == QModbusDataUnit::DiscreteInputs) {
        tv = ui->server_registers_->widget(1);
    } else if (table == QModbusDataUnit::InputRegisters) {
        tv = ui->server_registers_->widget(2);
    } else if (table == QModbusDataUnit::HoldingRegisters) {
        tv = ui->server_registers_->widget(3);
    } else {
        tv = ui->server_registers_->widget(3);
        qWarning() << "Get table view failed: unknow register type!";
    }

    return qobject_cast<QTableView *>(tv);
}

QList<quint16> xToolsModbusStudioUi::getTableValues(QTableView *tableView, int row, int count)
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

void xToolsModbusStudioUi::outputModbusReply(QModbusReply *reply, int functionCode)
{
    if (reply->error() != QModbusDevice::NoError) {
        outputMessage(reply->errorString(), true, "", "error");
        QMessageBox::warning(this, tr("Error occured"), reply->errorString());
        return;
    }

    int server_address = reply->serverAddress();
    if (reply->type() == QModbusReply::ReplyType::Raw) {
        QByteArray data = reply->rawResult().data();
        QString info = QString("(from address: %1, "
                               "function code: %2), "
                               "data unit: %3")
                           .arg(server_address)
                           .arg(functionCode)
                           .arg(QString::fromLatin1(xToolsByteArrayToHex(data, ' ')));
        outputMessage(info, false, RXCOLOR, RXFLAG);
    } else if (reply->type() == QModbusReply::ReplyType::Common) {
        QString info = ui->function_code_->currentText();
        outputMessage(info, false, RXCOLOR, RXFLAG);
    }
}

void xToolsModbusStudioUi::outputMessage(const QString &msg,
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
    ui->text_browser_->append(cookedMsg);
}

bool xToolsModbusStudioUi::isConnected()
{
    if (xToolsModbusStudio::Instance()->IsConnected(m_modbusDevice)) {
        return true;
    }

    QMessageBox::warning(this,
                         tr("Device is not Ready"),
                         tr("The modbus device is not ready, "
                            "please check settings and try again later!"));
    return false;
}

bool xToolsModbusStudioUi::writeSettingsArray(
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
