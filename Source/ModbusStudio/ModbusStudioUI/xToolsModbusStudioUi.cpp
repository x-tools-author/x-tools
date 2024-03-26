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

struct SAKModbusUiSettingKeys
{
    const QString device_index = "SAKModbus/deviceIndex";

    const QString port_name = "SAKModbus/portName";
    const QString parity = "SAKModbus/parity";
    const QString baud_rate = "SAKModbus/baudRate";
    const QString data_bits = "SAKModbus/dataBits";
    const QString stop_bits = "SAKModbus/stopBits";
    const QString custom_baud_rate = "SAKModbus/customBaudRate";

    const QString address = "SAKModbus/address";
    const QString port = "SAKModbus/port";
    const QString custom_address = "SAKModbus/customAddress";

    const QString client_timeout = "SAKModbus/clientTimeout";
    const QString client_repeat_time = "SAKModbus/clientRepeatTime";

    const QString server_is_busy = "SAKModbus/serverIsBusy";
    const QString server_just_listen = "SAKModbus/serverJustListen";
    const QString server_address = "SAKModbus/serverAddress";

    const QString function_code = "SAKModbus/functionCode";
    const QString target_address = "SAKModbus/targetAddress";
    const QString start_address = "SAKModbus/startAddress";
    const QString address_number = "SAKModbus/addressNumber";

    const QString send_history = "SAKModbus/sendHistory";
    const QString send_history_index = "SAKModbus/sendHistoryIndex";
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
    , modbus_device_(Q_NULLPTR)
    , register_model_(Q_NULLPTR)
    , key_ctx_(new SAKModbusUiSettingKeys)
{
    if (!settings_) {
        settings_ = xToolsSettings::instance();
    }

    ui->setupUi(this);

    InitComponents();
    InitSettings();
    InitSignals();

    UpdateUiState(false);
    OnDeviceTypeChanged();
    UpdateClientTableView();
    UpdateClientReadWriteButtonState();
}

xToolsModbusStudioUi::~xToolsModbusStudioUi()
{
    delete ui;
}

void xToolsModbusStudioUi::InitComponents()
{
    InitComponentDevices();
    InitComponentAddress();
    InitComponentPortName();
    InitComponnetBaudRate();
    InitComponnetDataBits();
    InitComponnetStopBits();
    InitComponnetParity();
    InitComponentFunctionCode();
    InitComponentRegisterTableView();
    InitComponentInput();
    InitComponentRegisterTabWidget();
}

void xToolsModbusStudioUi::InitComponentDevices()
{
    ui->device_list_->addItem(tr("RtuClient"), xToolsModbusStudio::kModbusRtuSerialClient);
    ui->device_list_->addItem(tr("RtuServer"), xToolsModbusStudio::kModbusRtuSerialServer);
    ui->device_list_->addItem(tr("TcpClient"), xToolsModbusStudio::kModbusTcpClient);
    ui->device_list_->addItem(tr("TcpServer"), xToolsModbusStudio::kModbusTcpServer);
}

void xToolsModbusStudioUi::InitComponentAddress()
{
    ui->address_combo_box->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->address_combo_box->addItem(address.toString());
        }
    }
}

void xToolsModbusStudioUi::InitComponentPortName()
{
    ui->port_name_->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui->port_name_->addItem(info.portName());
    }
}

void xToolsModbusStudioUi::InitComponnetBaudRate()
{
    ui->baud_rate_->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui->baud_rate_->addItem(QString::number(bd), bd);
    }
}

void xToolsModbusStudioUi::InitComponnetDataBits()
{
    ui->data_bits_->clear();
    ui->data_bits_->addItem("8", QSerialPort::Data8);
    ui->data_bits_->addItem("7", QSerialPort::Data7);
    ui->data_bits_->addItem("6", QSerialPort::Data6);
    ui->data_bits_->addItem("5", QSerialPort::Data5);
}

void xToolsModbusStudioUi::InitComponnetStopBits()
{
    ui->stop_bits_->clear();
    ui->stop_bits_->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stop_bits_->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->stop_bits_->addItem("2", QSerialPort::TwoStop);
}

void xToolsModbusStudioUi::InitComponnetParity()
{
    ui->parity_->clear();
    ui->parity_->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui->parity_->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui->parity_->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui->parity_->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui->parity_->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void xToolsModbusStudioUi::InitComponentFunctionCode()
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

void xToolsModbusStudioUi::InitComponentRegisterTableView()
{
    QTableView *table_view = CreateTableView(1, ui->client_registers_);
    register_model_ = qobject_cast<QStandardItemModel *>(table_view->model());
}

void xToolsModbusStudioUi::InitComponentInput()
{
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui->pdu_->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void xToolsModbusStudioUi::InitComponentRegisterTabWidget()
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

void xToolsModbusStudioUi::InitSettings()
{
    InitSettingsDevice();
    InitSettingsNetwork();
    InitSettingsSerialPort();
    InitSettingsClient();
    InitSettingsServer();
    InitSettingsClientOperations();
    InitSettingsInput();
}

void xToolsModbusStudioUi::InitSettingsDevice()
{
    int deviceIndex = settings_->value(key_ctx_->device_index).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui->device_list_->count()) {
        ui->device_list_->setCurrentIndex(deviceIndex);
    }
}

void xToolsModbusStudioUi::InitSettingsNetwork()
{
    QString address = settings_->value(key_ctx_->address).toString();
    ui->address_combo_box->setCurrentText(address);

    QVariant portValiant = settings_->value(key_ctx_->port);
    int port = portValiant.toInt();
    if (portValiant.isNull()) {
        port = 502;
    }
    ui->port_spin_box->setValue(port);
}

void xToolsModbusStudioUi::InitSettingsSerialPort()
{
    auto SetComboBoxIndex = [](int index, QComboBox *cb) {
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    };

    QString portName = settings_->value(key_ctx_->port_name).toString();
    int index = ui->port_name_->findText(portName);
    SetComboBoxIndex(index, ui->port_name_);

    index = settings_->value(key_ctx_->parity).toInt();
    SetComboBoxIndex(index, ui->parity_);

    QString bd = settings_->value(key_ctx_->baud_rate).toString();
    ui->baud_rate_->setCurrentText(bd);

    index = settings_->value(key_ctx_->data_bits).toInt();
    SetComboBoxIndex(index, ui->data_bits_);

    index = settings_->value(key_ctx_->stop_bits).toInt();
    SetComboBoxIndex(index, ui->stop_bits_);
}

void xToolsModbusStudioUi::InitSettingsClient()
{
    int timeout = settings_->value(key_ctx_->client_timeout).toInt();
    ui->timeout_->setValue(timeout < 100 ? 100 : timeout);

    const QString key = key_ctx_->client_repeat_time;
    int repeatTimes = settings_->value(key).toInt();
    ui->repeat_time_->setValue(repeatTimes);
}

void xToolsModbusStudioUi::InitSettingsServer()
{
    bool isBusy = settings_->value(key_ctx_->server_is_busy).toBool();
    ui->device_busy_->setChecked(isBusy);

    QString key = key_ctx_->server_just_listen;
    bool just_listen = settings_->value(key).toBool();
    ui->listen_only_mode_->setChecked(just_listen);

    int address = settings_->value(key_ctx_->server_address).toInt();
    ui->server_address->setValue(address);
}

void xToolsModbusStudioUi::InitSettingsClientOperations()
{
    int index = settings_->value(key_ctx_->function_code).toInt();
    if (index >= 0 && index < ui->function_code_->count()) {
        ui->function_code_->setCurrentIndex(index);
    }

    int address = settings_->value(key_ctx_->target_address).toInt();
    ui->device_address_->setValue(address);

    int start = settings_->value(key_ctx_->start_address).toInt();
    ui->start_address_->setValue(start);

    int number = settings_->value(key_ctx_->address_number).toInt();
    ui->quantity_->setValue(number);
}

void xToolsModbusStudioUi::InitSettingsInput()
{
    ui->pdu_->clear();
    settings_->beginReadArray(key_ctx_->send_history);
    for (int i = 0; i < 10; i++) {
        settings_->setArrayIndex(i);
        QString text = settings_->value(key_ctx_->pdu).toString();
        if (!text.isEmpty()) {
            ui->pdu_->addItem(text);
        }
    }
    settings_->endArray();

    int index = settings_->value(key_ctx_->send_history_index).toInt();
    ui->pdu_->setCurrentIndex(index - 1);
}

void xToolsModbusStudioUi::InitSignals()
{
    InitSignalsDevice();
    InitSignalsNetworking();
    InitSignalsSerialPort();
    InitSignalsClient();
    InitSignalsServer();
    InitSignalsClientOperations();
}

void xToolsModbusStudioUi::InitSignalsDevice()
{
    connect(ui->device_list_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::OnDeviceTypeChanged);
    connect(ui->open_button_, &QPushButton::clicked, this, &xToolsModbusStudioUi::OnOpenClicked);
    connect(ui->cloese_button_, &QPushButton::clicked, this, &xToolsModbusStudioUi::OnCloseClicked);
}

void xToolsModbusStudioUi::InitSignalsNetworking()
{
    connect(ui->address_combo_box,
            &QComboBox::currentTextChanged,
            this,
            &xToolsModbusStudioUi::OnAddressChanged);
    connect(ui->port_spin_box,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnPortChanged);
}

void xToolsModbusStudioUi::InitSignalsSerialPort()
{
    connect(ui->port_name_, &QComboBox::currentTextChanged, this, &xToolsModbusStudioUi::OnPortNameChanged);
    connect(ui->parity_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::OnParityChanged);
    connect(ui->baud_rate_, &QComboBox::currentTextChanged, this, &xToolsModbusStudioUi::OnBaudRateChanged);
    connect(ui->data_bits_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::OnDataBitsChanged);
    connect(ui->stop_bits_,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsModbusStudioUi::OnStopBistChanged);
}

void xToolsModbusStudioUi::InitSignalsClient()
{
    connect(ui->timeout_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnClientTimeoutChanged);
    connect(ui->repeat_time_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnClientRepeatTimeChanged);
}

void xToolsModbusStudioUi::InitSignalsServer()
{
    connect(ui->device_busy_, &QCheckBox::clicked, this, &xToolsModbusStudioUi::OnServerIsBusyChanged);
    connect(ui->listen_only_mode_,
            &QCheckBox::clicked,
            this,
            &xToolsModbusStudioUi::OnServerJustListenChanged);
    connect(ui->server_address,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnServerAddressChanged);
}

void xToolsModbusStudioUi::InitSignalsClientOperations()
{
    connect(ui->function_code_,
            &QComboBox::currentTextChanged,
            this,
            &xToolsModbusStudioUi::OnFunctionCodeChanged);
    connect(ui->device_address_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnTargetAddressChanged);
    connect(ui->start_address_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnStartAddressChanged);
    connect(ui->quantity_,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsModbusStudioUi::OnAddressNumberChanged);
    connect(ui->read_, &QPushButton::clicked, this, &xToolsModbusStudioUi::OnReadClicked);
    connect(ui->write_, &QPushButton::clicked, this, &xToolsModbusStudioUi::OnWriteClicked);
    connect(ui->send_, &QPushButton::clicked, this, &xToolsModbusStudioUi::OnSendClicked);
}

void xToolsModbusStudioUi::OnErrorOccurred()
{
    outputMessage(modbus_device_->errorString(), true, "", "error");
    if (modbus_device_->error() == QModbusDevice::ConnectionError) {
        const QString msg = modbus_device_->errorString();
        OnCloseClicked();
        QMessageBox::warning(this, tr("Error occured"), msg);
    }
}

void xToolsModbusStudioUi::OnDeviceTypeChanged()
{
    int type = ui->device_list_->currentData().toInt();
    bool isSerial = (type == xToolsModbusStudio::kModbusRtuSerialClient
                     || type == xToolsModbusStudio::kModbusRtuSerialServer);
    bool isClient = (type == xToolsModbusStudio::kModbusRtuSerialClient
                     || type == xToolsModbusStudio::kModbusTcpClient);

    // Hide ui component first then show ui component,
    // or the window will be resize to the max size of default.
    if (isSerial) {
        ui->networkGroupBox->setHidden(true);
        ui->serialPortGroupBox->setHidden(false);
    } else {
        ui->serialPortGroupBox->setHidden(true);
        ui->networkGroupBox->setHidden(false);
    }

    if (isClient) {
        ui->console_->setVisible(true);
        ui->serverGroupBox->setHidden(true);
        ui->registersGroupBox->setHidden(true);
        ui->clientOperationsGroupBox->setHidden(false);
        ui->clientCustomCommandGroupBox->setHidden(false);
        ui->clientRegistersGroupBox->setHidden(false);
    } else {
        ui->console_->setVisible(false);
        ui->clientCustomCommandGroupBox->setHidden(true);
        ui->clientRegistersGroupBox->setHidden(true);
        ui->clientOperationsGroupBox->setHidden(true);
        ui->serverGroupBox->setHidden(false);
        ui->registersGroupBox->setHidden(false);
    }

    settings_->setValue(key_ctx_->device_index, type);
}

void xToolsModbusStudioUi::OnCloseClicked()
{
    xToolsModbusStudio::Instance()->DeleteModbusDevuce(&modbus_device_);
    UpdateUiState(false);
}

void xToolsModbusStudioUi::OnOpenClicked()
{
    ui->open_button_->setEnabled(false);
    xToolsModbusStudio::Instance()->DeleteModbusDevuce(&modbus_device_);

    modbus_device_ = CreateModbusDevice();

    if (xToolsModbusStudio::Instance()->IsServerDevice(modbus_device_)) {
        if (!UpdateServerMap(modbus_device_)) {
            ui->open_button_->setEnabled(true);
            qCWarning(kLoggingCategory) << "Can not reset server map!";
            return;
        }

        UpdateServerParameters();

        QModbusServer *server = qobject_cast<QModbusServer *>(modbus_device_);
        UpdateServerRegistersData();
        connect(server, &QModbusServer::dataWritten, this, &xToolsModbusStudioUi::OnDateWritten);
    } else if (xToolsModbusStudio::Instance()->IsClientDevice(modbus_device_)) {
        UpdateClientParameters();
    } else {
        ui->open_button_->setEnabled(true);
        qCWarning(kLoggingCategory) << "Can not create modbus devices!";
        return;
    }

    connect(modbus_device_, &QModbusDevice::errorOccurred, this, &xToolsModbusStudioUi::OnErrorOccurred);
    xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
    bool connected = factory->ConnectDeivce(modbus_device_);
    if (!connected) {
        QString errStr = modbus_device_->errorString();
        QString info = tr("Can not open device: %1."
                          "Please check the parameters and try again!")
                           .arg(errStr);
        QMessageBox::warning(this, tr("Can not open device"), info);
    }

    UpdateUiState(connected);
}

void xToolsModbusStudioUi::OnAddressChanged()
{
    settings_->setValue(key_ctx_->address, ui->address_combo_box->currentText());
}

void xToolsModbusStudioUi::OnPortChanged()
{
    settings_->setValue(key_ctx_->port, ui->port_spin_box->value());
}

void xToolsModbusStudioUi::OnPortNameChanged()
{
    settings_->setValue(key_ctx_->port_name, ui->port_name_->currentText());
}

void xToolsModbusStudioUi::OnParityChanged()
{
    settings_->setValue(key_ctx_->parity, ui->parity_->currentIndex());
}

void xToolsModbusStudioUi::OnBaudRateChanged()
{
    settings_->setValue(key_ctx_->baud_rate, ui->baud_rate_->currentText());
}

void xToolsModbusStudioUi::OnDataBitsChanged()
{
    settings_->setValue(key_ctx_->data_bits, ui->data_bits_->currentIndex());
}

void xToolsModbusStudioUi::OnStopBistChanged()
{
    settings_->setValue(key_ctx_->stop_bits, ui->stop_bits_->currentIndex());
}

void xToolsModbusStudioUi::OnInvokeRefresh()
{
    InitComponentPortName();
}

void xToolsModbusStudioUi::OnClientTimeoutChanged()
{
    settings_->setValue(key_ctx_->client_timeout, ui->timeout_->value());
    UpdateClientParameters();
}

void xToolsModbusStudioUi::OnClientRepeatTimeChanged()
{
    settings_->setValue(key_ctx_->client_repeat_time, ui->repeat_time_->value());
    UpdateClientParameters();
}

void xToolsModbusStudioUi::OnServerIsBusyChanged()
{
    settings_->setValue(key_ctx_->server_is_busy, ui->device_busy_->isChecked());
    UpdateServerParameters();
}

void xToolsModbusStudioUi::OnServerJustListenChanged()
{
    settings_->setValue(key_ctx_->server_just_listen, ui->listen_only_mode_->isChecked());
    UpdateServerParameters();
}

void xToolsModbusStudioUi::OnServerAddressChanged()
{
    settings_->setValue(key_ctx_->server_address, ui->server_address->value());
    UpdateServerParameters();
}

void xToolsModbusStudioUi::OnFunctionCodeChanged()
{
    settings_->setValue(key_ctx_->function_code, ui->function_code_->currentIndex());
    UpdateClientReadWriteButtonState();
}

void xToolsModbusStudioUi::OnTargetAddressChanged()
{
    settings_->setValue(key_ctx_->target_address, ui->device_address_->value());
}

void xToolsModbusStudioUi::OnStartAddressChanged()
{
    settings_->setValue(key_ctx_->start_address, ui->start_address_->value());
    UpdateClientTableView();
}

void xToolsModbusStudioUi::OnAddressNumberChanged()
{
    settings_->setValue(key_ctx_->address_number, ui->quantity_->value());
    UpdateClientTableView();
}

void xToolsModbusStudioUi::OnReadClicked()
{
    if (!IsConnected()) {
        return;
    }

    if (!xToolsModbusStudio::Instance()->IsClientDevice(modbus_device_)) {
        return;
    }

    int register_type = ui->function_code_->currentData().toInt();
    quint16 start_address = ui->start_address_->value();
    quint16 quantity = ui->quantity_->value();
    quint16 server_address = ui->device_address_->value();
    quint8 function_code = GetClientFunctionCode();

    qCInfo(kLoggingCategory) << "[SendReadRequest]"
                             << "register type:" << register_type
                             << "start address:" << start_address << "quantity:" << quantity
                             << "server address:" << server_address;

    typedef QModbusDataUnit::RegisterType RegisterType;
    RegisterType type = static_cast<RegisterType>(register_type);
    QModbusDataUnit data_unit(type, start_address, quantity);
    QModbusClient *client = qobject_cast<QModbusClient *>(modbus_device_);
    QModbusReply *reply = client->sendReadRequest(data_unit, server_address);
    if (!xToolsModbusStudio::Instance()->IsValidModbusReply(reply)) {
        return;
    }

    QString info = ui->function_code_->currentText();
    outputMessage(info, false, TXCOLOR, TXFLAG);
    connect(reply, &QModbusReply::finished, this, [=]() {
        OutputModbusReply(reply, function_code);

        if (reply->error() == QModbusDevice::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            UpdateClientTableViewData(reply->result().values());
#else
            UpdateClientTableViewData(vectorTolist(reply->result().values()));
#endif
            reply->deleteLater();
        }
    });
}

void xToolsModbusStudioUi::OnWriteClicked()
{
    if (!IsConnected()) {
        return;
    }

    int registerType = ui->function_code_->currentData().toInt();
    int start_address = ui->start_address_->value();
    int server_address = ui->device_address_->value();
    quint8 function_code = GetClientFunctionCode();
    QList<quint16> values = GetClientRegisterValue();
    xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
    QModbusReply *reply = factory->SendWriteRequest(modbus_device_,
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
            OutputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = ui->function_code_->currentText();
        outputMessage(info, false, TXCOLOR, TXFLAG);
    }
}

void xToolsModbusStudioUi::OnSendClicked()
{
    if (!IsConnected()) {
        return;
    }

    quint8 server_address = ui->device_address_->value();
    QByteArray pdu = GetClientPdu();
    QByteArray data = pdu;
    if (!data.isEmpty()) {
        data = data.remove(0, 1);
    }

    int function_code = int(QModbusDataUnit::Invalid);
    if (!pdu.isEmpty()) {
        function_code = pdu.at(0);
    }
    xToolsModbusStudio *factory = xToolsModbusStudio::Instance();
    QModbusReply *reply = factory->SendRawRequest(modbus_device_,
                                                  server_address,
                                                  function_code,
                                                  data);

    qCWarning(kLoggingCategory) << "Send raw request:"
                                << "server address:" << server_address
                                << "function code:" << function_code
                                << "data:" << QString(xToolsByteArrayToHex(pdu, ' '));
    if (xToolsModbusStudio::Instance()->IsValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=]() {
            OutputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = "pdu(No server address, no crc):";
        info += QString(xToolsByteArrayToHex(pdu, ' '));
        outputMessage(info, false, TXCOLOR, TXFLAG);
    }

    // Update settings data.
    int index = settings_->value(key_ctx_->send_history_index).toInt();
    bool ret = WriteSettingsArray(key_ctx_->send_history,
                                  key_ctx_->pdu,
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
    settings_->setValue(key_ctx_->send_history_index, index);
}

void xToolsModbusStudioUi::OnDateWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    qCInfo(kLoggingCategory) << "Data written:"
                             << "table:" << table << "start address:" << address << "size:" << size;
    QTableView *tv = GetTableView(table);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer *>(modbus_device_);
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

void xToolsModbusStudioUi::OnItemChanged(QStandardItem *item)
{
    if (!item) {
        return;
    }

    if (item->column() != 1) {
        return;
    }

    if (xToolsModbusStudio::Instance()->IsServerDevice(modbus_device_)) {
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
        xToolsModbusStudio::Instance()->SetServerData(modbus_device_, table, address, value);
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

QTableView *xToolsModbusStudioUi::CreateTableView(int row_count, QTableView *table_view)
{
    if (!table_view) {
        table_view = new QTableView(this);
    }

    QHeaderView *hv = table_view->horizontalHeader();
    QStandardItemModel *model = new QStandardItemModel(table_view);
    QStringList labels = QStringList() << tr("Address") << tr("Value") << tr("Description");
    model->setHorizontalHeaderLabels(labels);
    model->setColumnCount(3);
    model->setRowCount(row_count);
    table_view->setModel(model);
    table_view->verticalHeader()->hide();
    table_view->setItemDelegateForColumn(0, new ReadOnlyDelegate(table_view));
    UpdateClientTableViewAddress(table_view, 0);
    hv->setStretchLastSection(true);

    // Set the default value to 0.
    model->blockSignals(true);
    for (int row = 0; row < row_count; row++) {
        QModelIndex index = model->index(row, 1);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, "0000");
        model->setItemData(index, roles);

        QStandardItem *item = model->item(row, 1);
        item->setTextAlignment(Qt::AlignCenter);
    }
    model->blockSignals(false);

    connect(model, &QStandardItemModel::itemChanged, this, &xToolsModbusStudioUi::OnItemChanged);

    return table_view;
}

void xToolsModbusStudioUi::UpdateUiState(bool connected)
{
    ui->device_list_->setEnabled(!connected);
    ui->cloese_button_->setEnabled(connected);
    ui->open_button_->setEnabled(!connected);
    ui->networkGroupBox->setEnabled(!connected);
    ui->serialPortGroupBox->setEnabled(!connected);
#if 0
    ui->clientGroupBox->setEnabled(!connected);
    ui->serverGroupBox->setEnabled(!connected);
#endif
}

void xToolsModbusStudioUi::UpdateClientTableView()
{
    int number = ui->quantity_->value();
    int rowCount = register_model_->rowCount();
    if (number > rowCount) {
        register_model_->insertRows(rowCount, number - rowCount);
    } else if (number < rowCount) {
        register_model_->removeRows(number, rowCount - number);
    }

    // Update address.
    int start_address = ui->start_address_->value();
    UpdateClientTableViewAddress(ui->client_registers_, start_address);
}

void xToolsModbusStudioUi::UpdateClientTableViewData(const QList<quint16> &values)
{
    for (int row = 0; row < values.count(); row++) {
        int value = values.at(row);
        QModelIndex index = register_model_->index(row, 1);
        QMap<int, QVariant> roles;
        QString str = QString("%1").arg(QString::number(value, 16), 4, '0');
        roles.insert(Qt::DisplayRole, str);
        register_model_->setItemData(index, roles);
        QStandardItem *item = register_model_->item(row, 1);
        if (item) {
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    // Refresh the view, or the new value will not be show.
    ui->client_registers_->viewport()->update();
}

void xToolsModbusStudioUi::UpdateClientReadWriteButtonState()
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

void xToolsModbusStudioUi::UpdateClientParameters()
{
    int timeout = ui->timeout_->value();
    int repeat_time = ui->repeat_time_->value();
    xToolsModbusStudio::Instance()->SetClientDeviceParameters(modbus_device_, timeout, repeat_time);
}

void xToolsModbusStudioUi::UpdateClientTableViewAddress(QTableView *view, int start_address)
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

void xToolsModbusStudioUi::UpdateServerParameters()
{
    bool device_busy = ui->device_busy_->isChecked();
    bool listen_only_mode = ui->listen_only_mode_->isChecked();
    int address = ui->server_address->value();
    xToolsModbusStudio::Instance()->SetServerDeviceParameters(modbus_device_,
                                                            address,
                                                            device_busy,
                                                            listen_only_mode);
}

bool xToolsModbusStudioUi::UpdateServerMap(QModbusDevice *server)
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

void xToolsModbusStudioUi::UpdateServerRegistersData()
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
            qCWarning(kLoggingCategory) << "Unknown register type.";
            continue;
        }

        for (int row = 0; row < model->rowCount(); row++) {
            QStandardItem *item = model->item(row, 1);
            quint16 value = item ? item->text().toInt(Q_NULLPTR, 16) : 0;
            auto table = static_cast<QModbusDataUnit::RegisterType>(type);
            xToolsModbusStudio::Instance()->SetServerData(modbus_device_, table, row, value, false);
        }
    }
}

quint8 xToolsModbusStudioUi::GetClientFunctionCode()
{
    QString txt = ui->function_code_->currentText();
    QStringList list = txt.split('-', xToolsSkipEmptyParts);
    if (list.length()) {
        return list.first().toInt(Q_NULLPTR, 16);
    }

    return 0;
}

QList<quint16> xToolsModbusStudioUi::GetClientRegisterValue()
{
    QList<quint16> values;
    for (int row = 0; row < register_model_->rowCount(); row++) {
        QStandardItem *item = register_model_->item(row, 1);
        if (item) {
            QString text = item->text();
            values.append(text.toInt(Q_NULLPTR, 16));
        } else {
            values.append(0);
        }
    }

    return values;
}

QByteArray xToolsModbusStudioUi::GetClientPdu()
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

QTableView *xToolsModbusStudioUi::GetTableView(QModbusDataUnit::RegisterType table)
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
        qCWarning(kLoggingCategory) << "Get table view failed: unknow register type!";
    }

    return qobject_cast<QTableView *>(tv);
}

QList<quint16> xToolsModbusStudioUi::GetTableValues(QTableView *table_view, int row, int count)
{
    if (!table_view) {
        qCWarning(kLoggingCategory) << "Table view can not be null!";
        return QList<quint16>();
    }

    auto *model = qobject_cast<QStandardItemModel *>(table_view->model());
    if (!model) {
        qCWarning(kLoggingCategory) << "Model can not be null!";
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

void xToolsModbusStudioUi::OutputModbusReply(QModbusReply *reply, int function_code)
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
                           .arg(function_code)
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

bool xToolsModbusStudioUi::IsConnected()
{
    if (xToolsModbusStudio::Instance()->IsConnected(modbus_device_)) {
        return true;
    }

    QMessageBox::warning(this,
                         tr("Device is not Ready"),
                         tr("The modbus device is not ready, "
                            "please check settings and try again later!"));
    return false;
}

bool xToolsModbusStudioUi::WriteSettingsArray(
    const QString &group, const QString &key, const QString &value, int index, int max_index)
{
    settings_->beginWriteArray(group);
    for (int i = 0; i < max_index; i++) {
        settings_->setArrayIndex(i);
        QString v = settings_->value(key).toString();
        if (v == value) {
            settings_->endArray();
            return false;
        }
    }

    settings_->setArrayIndex(index);
    settings_->setValue(key, value);
    settings_->endArray();
    return true;
}
