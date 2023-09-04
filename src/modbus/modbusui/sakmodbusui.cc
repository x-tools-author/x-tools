/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QtEndian>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QDateTime>
#include <QSerialPort>
#include <QPushButton>
#include <QMessageBox>
#include <QTextBrowser>
#include <QHostAddress>
#include <QModbusServer>
#include <QItemDelegate>
#include <QAbstractSocket>
#include <QSerialPortInfo>
#include <QNetworkInterface>
#include <QRegularExpression>
#include <QStandardItemModel>
#include <QRegularExpression>
#include <QNetworkAddressEntry>

#include <QModbusTcpClient>
#include <QModbusTcpServer>
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
#include <QModbusRtuSerialSlave>
#include <QModbusRtuSerialMaster>
#else
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#endif

#include "sakmodbusui.h"
#include "SAKSettings.hh"
#include "sakmodbusfactory.h"

#include "ui_sakmodbusui.h"

#define RXCOLOR "green"
#define TXCOLOR "blue"
#define RXFLAG "rx:"
#define TXFLAG "tx:"
#define MAX_HISTORY_INDEX 9

class ReadOnlyDelegate: public QItemDelegate {
public:
    ReadOnlyDelegate(QWidget *parent = Q_NULLPTR):QItemDelegate(parent){}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        Q_UNUSED(parent);
        Q_UNUSED(option);
        Q_UNUSED(index);
        return Q_NULLPTR;
    }
};

SAKModbusUi::SAKModbusUi(QWidget *parent)
    : QWidget{parent}
    , ui_(new Ui::SAKModbusUi)
    , modbus_device_(Q_NULLPTR)
    , register_model_(Q_NULLPTR)
    , logging_category_ ("SAK.Modbus"){
    if (!settings_) {
        settings_ = SAKSettings::instance();
    }

    ui_->setupUi(this);

    InitComponents();
    InitSettings();
    InitSignals();

    UpdateUiState(false);
    OnDeviceTypeChanged();
    ClientUpdateTable();
    ClientUpdateRWBtState();
}

SAKModbusUi::~SAKModbusUi() {
    delete ui_;
}

void SAKModbusUi::InitComponents() {
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

void SAKModbusUi::InitComponentDevices() {
    ui_->device_list_->addItem(tr("RtuClient"), ModbusDeviceRtuSerialClient);
    ui_->device_list_->addItem(tr("RtuServer"), ModbusDeviceRtuSerialServer);
    ui_->device_list_->addItem(tr("TcpClient"), ModbusDeviceTcpClient);
    ui_->device_list_->addItem(tr("TcpServer"), ModbusDeviceTcpServer);
}

void SAKModbusUi::InitComponentAddress() {
    ui_->address_combo_box->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui_->address_combo_box->addItem(address.toString());
        }
    }
}

void SAKModbusUi::InitComponentPortName() {
    ui_->port_name_->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui_->port_name_->addItem(info.portName());
    }
}

void SAKModbusUi::InitComponnetBaudRate() {
    ui_->baud_rate_->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui_->baud_rate_->addItem(QString::number(bd), bd);
    }
}

void SAKModbusUi::InitComponnetDataBits() {
    ui_->data_bits_->clear();
    ui_->data_bits_->addItem("8", QSerialPort::Data8);
    ui_->data_bits_->addItem("7", QSerialPort::Data7);
    ui_->data_bits_->addItem("6", QSerialPort::Data6);
    ui_->data_bits_->addItem("5", QSerialPort::Data5);
}

void SAKModbusUi::InitComponnetStopBits() {
    ui_->stop_bits_->clear();
    ui_->stop_bits_->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsComboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui_->stop_bits_->addItem("2", QSerialPort::TwoStop);
}

void SAKModbusUi::InitComponnetParity() {
    ui_->parity_->clear();
    ui_->parity_->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui_->parity_->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui_->parity_->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui_->parity_->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui_->parity_->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void SAKModbusUi::InitComponentFunctionCode() {
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

    ui_->function_code_->addItem(str0x01, func0x01);
    ui_->function_code_->addItem(str0x02, func0x02);
    ui_->function_code_->addItem(str0x03, func0x03);
    ui_->function_code_->addItem(str0x04, func0x04);
    ui_->function_code_->addItem(str0x05, func0x05);
    ui_->function_code_->addItem(str0x06, func0x06);
    ui_->function_code_->addItem(str0x0f, func0x0f);
    ui_->function_code_->addItem(str0x10, func0x10);
}

void SAKModbusUi::InitComponentRegisterTableView() {
    QTableView *tv = TableViewInit(1, ui_->client_registers_);
    register_model_ = reinterpret_cast<QStandardItemModel*>(tv->model());
}

void SAKModbusUi::InitComponentInput() {
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui_->pdu_->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void SAKModbusUi::InitComponentRegisterTabWidget() {
    QTabWidget *tw = ui_->server_registers_;
    QStringList titles;
    titles << tr("DiscreteInputs") << tr("Coils")
           << tr("InputRegisters") << tr("HoldingRegisters");
    for (QString &title : titles) {
        QTableView *tv = TableViewInit(UINT16_MAX + 1, Q_NULLPTR);
        tv->verticalHeader()->hide();
        tw->addTab(tv, title);
    }
}

void SAKModbusUi::InitSettings() {
    InitSettingsDevice();
    InitSettingsNetwork();
    InitSettingsSerialPort();
    InitSettingsClient();
    InitSettingsServer();
    InitSettingsClientOperations();
    InitSettingsInput();
}

void SAKModbusUi::InitSettingsDevice() {
    int deviceIndex = settings_->value(mSettingsKeyCtx.deviceIndex).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui_->device_list_->count()) {
        ui_->device_list_->setCurrentIndex(deviceIndex);
    }
}

void SAKModbusUi::InitSettingsNetwork() {
    QString address = settings_->value(mSettingsKeyCtx.address).toString();
    ui_->address_combo_box->setCurrentText(address);

    QVariant portValiant = settings_->value(mSettingsKeyCtx.port);
    int port = portValiant.toInt();
    if (portValiant.isNull()) {
        port = 502;
    }
    ui_->port_spin_box->setValue(port);
}

void SAKModbusUi::InitSettingsSerialPort() {
    auto SetComboBoxIndex = [](int index, QComboBox *cb){
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    };

    QString portName = settings_->value(mSettingsKeyCtx.portName).toString();
    int index = ui_->port_name_->findText(portName);
    SetComboBoxIndex(index, ui_->port_name_);

    index = settings_->value(mSettingsKeyCtx.parity).toInt();
    SetComboBoxIndex(index, ui_->parity_);

    QString bd = settings_->value(mSettingsKeyCtx.baudRate).toString();
    ui_->baud_rate_->setCurrentText(bd);

    index = settings_->value(mSettingsKeyCtx.dataBits).toInt();
    SetComboBoxIndex(index, ui_->data_bits_);

    index = settings_->value(mSettingsKeyCtx.stopBits).toInt();
    SetComboBoxIndex(index, ui_->stop_bits_);
}

void SAKModbusUi::InitSettingsClient() {
    int timeout = settings_->value(mSettingsKeyCtx.clientTimeout).toInt();
    ui_->timeout_->setValue(timeout < 100 ? 100 : timeout);

    const QString key = mSettingsKeyCtx.clientRepeatTime;
    int repeatTimes =settings_->value(key).toInt();
    ui_->repeat_time_->setValue(repeatTimes);
}

void SAKModbusUi::InitSettingsServer() {
    bool isBusy = settings_->value(mSettingsKeyCtx.serverIsBusy).toBool();
    ui_->is_busy_->setChecked(isBusy);

    QString key = mSettingsKeyCtx.serverJustListen;
    bool just_listen = settings_->value(key).toBool();
    ui_->just_listen_->setChecked(just_listen);

    int address = settings_->value(mSettingsKeyCtx.serverAddress).toInt();
    ui_->server_address->setValue(address);
}

void SAKModbusUi::InitSettingsClientOperations() {
    int index = settings_->value(mSettingsKeyCtx.functionCode).toInt();
    if (index >= 0 && index < ui_->function_code_->count()) {
        ui_->function_code_->setCurrentIndex(index);
    }

    int address = settings_->value(mSettingsKeyCtx.targetAddress).toInt();
    ui_->device_address_->setValue(address);

    int start = settings_->value(mSettingsKeyCtx.startAddress).toInt();
    ui_->start_address_->setValue(start);

    int number = settings_->value(mSettingsKeyCtx.addressNumber).toInt();
    ui_->quantity_->setValue(number);
}

void SAKModbusUi::InitSettingsInput() {
    ui_->pdu_->clear();
    settings_->beginReadArray(mSettingsKeyCtx.sendHistory);
    for (int i = 0; i < 10; i++) {
        settings_->setArrayIndex(i);
        QString text = settings_->value(mSettingsKeyCtx.pdu).toString();
        if (!text.isEmpty()) {
            ui_->pdu_->addItem(text);
        }
    }
    settings_->endArray();

    int index = settings_->value(mSettingsKeyCtx.sendHistoryIndex).toInt();
    ui_->pdu_->setCurrentIndex(index - 1);
}

void SAKModbusUi::InitSignals() {
    InitSignalsDevice();
    InitSignalsNetworking();
    InitSignalsSerialPort();
    InitSignalsClient();
    InitSignalsServer();
    InitSignalsClientOperations();
}

void SAKModbusUi::InitSignalsDevice() {
    connect(ui_->device_list_,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnDeviceTypeChanged);
    connect(ui_->open_button_, &QPushButton::clicked,
            this, &SAKModbusUi::OnOpenClicked);
    connect(ui_->cloese_button_, &QPushButton::clicked,
            this, &SAKModbusUi::OnInvokeClose);
}

void SAKModbusUi::InitSignalsNetworking() {
    connect(ui_->address_combo_box, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnAddressChanged);
    connect(ui_->port_spin_box, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnPortChanged);
}

void SAKModbusUi::InitSignalsSerialPort() {
    connect(ui_->port_name_, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnPortNameChanged);
    connect(ui_->parity_,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnParityChanged);
    connect(ui_->baud_rate_, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnBaudRateChanged);
    connect(ui_->data_bits_,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnDataBitsChanged);
    connect(ui_->stop_bits_,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnStopBistChanged);
}

void SAKModbusUi::InitSignalsClient() {
    connect(ui_->timeout_,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnClientTimeoutChanged);
    connect(ui_->repeat_time_,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnClientRepeatTimeChanged);
}

void SAKModbusUi::InitSignalsServer() {
    connect(ui_->is_busy_, &QCheckBox::clicked,
            this, &SAKModbusUi::OnServerIsBusyChanged);
    connect(ui_->just_listen_, &QCheckBox::clicked,
            this, &SAKModbusUi::OnServerJustListenChanged);
    connect(ui_->server_address,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnServerAddressChanged);
}

void SAKModbusUi::InitSignalsClientOperations() {
    connect(ui_->function_code_, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnFunctionCodeChanged);
    connect(ui_->device_address_,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnTargetAddressChanged);
    connect(ui_->start_address_,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnStartAddressChanged);
    connect(ui_->quantity_,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnAddressNumberChanged);
    connect(ui_->read_, &QPushButton::clicked,
            this, &SAKModbusUi::OnReadClicked);
    connect(ui_->write_, &QPushButton::clicked,
            this, &SAKModbusUi::OnWriteClicked);
    connect(ui_->send_, &QPushButton::clicked,
            this, &SAKModbusUi::OnSendClicked);
}

void SAKModbusUi::OnErrorOccurred() {
    OutputMessage(modbus_device_->errorString(), true, "", "error");
    if (modbus_device_->error() == QModbusDevice::ConnectionError) {
        const QString msg = modbus_device_->errorString();
        OnInvokeClose();
        ShowMessageBox(tr("Error occured"), msg);
    }
#if 0
    SAKMDF->closeDevice(mModbusDevice);
    mModbusDevice = Q_NULLPTR;
#endif
}

void SAKModbusUi::OnDeviceTypeChanged() {
    int type = ui_->device_list_->currentData().toInt();
    bool isSerial = (type == ModbusDeviceRtuSerialClient
                     || type == ModbusDeviceRtuSerialServer);
    bool isClient = (type == ModbusDeviceRtuSerialClient
                     || type == ModbusDeviceTcpClient);

    // Hide ui component first then show ui component,
    // or the window will be resize to the max size of default.
    if (isSerial) {
        ui_->networkGroupBox->setHidden(true);
        ui_->serialPortGroupBox->setHidden(false);
    } else {
        ui_->serialPortGroupBox->setHidden(true);
        ui_->networkGroupBox->setHidden(false);
    }

    if (isClient) {
        ui_->serverGroupBox->setHidden(true);
        ui_->registersGroupBox->setHidden(true);
        ui_->clientOperationsGroupBox->setHidden(false);
        ui_->clientCustomCommandGroupBox->setHidden(false);
        ui_->clientRegistersGroupBox->setHidden(false);
    } else {
        ui_->clientCustomCommandGroupBox->setHidden(true);
        ui_->clientRegistersGroupBox->setHidden(true);
        ui_->clientOperationsGroupBox->setHidden(true);
        ui_->serverGroupBox->setHidden(false);
        ui_->registersGroupBox->setHidden(false);
    }

    settings_->setValue(mSettingsKeyCtx.deviceIndex, type);
}

void SAKModbusUi::OnInvokeClose() {
    if (modbus_device_) {
        if (modbus_device_->inherits("QModbusDevice")) {
            modbus_device_->disconnectDevice();
            modbus_device_->deleteLater();
        }
    }

    modbus_device_ = Q_NULLPTR;
    UpdateUiState(false);
}

void SAKModbusUi::OnOpenClicked() {
    ui_->open_button_->setEnabled(false);
    if (modbus_device_) {
        modbus_device_->disconnectDevice();
        modbus_device_->deleteLater();
        modbus_device_ = Q_NULLPTR;
    }

    int type = ui_->device_list_->currentData().toInt();
    if (type == ModbusDeviceRtuSerialClient
        || type == ModbusDeviceRtuSerialServer) {
        QString port_name = ui_->port_name_->currentText();
        int parity = ui_->parity_->currentData().toInt();
        int baud_rate = ui_->baud_rate_->currentData().toInt();
        int data_bits = ui_->data_bits_->currentData().toInt();
        int stop_bits = ui_->stop_bits_->currentData().toInt();

        qCInfo(logging_category_) << "[RTU Parameters]port name:" << port_name
                                  << "parity" << parity
                                  << "baud rate:" << baud_rate
                                  << "data bits:" << data_bits
                                  << "stop bits" << stop_bits;
        modbus_device_ = CreateRtuSerialDevice(type, port_name, parity,
                                               baud_rate, data_bits,
                                               stop_bits);
    } else {
        QString address = ui_->address_combo_box->currentText();
        int port = ui_->port_spin_box->value();

        qCInfo(logging_category_) << "[TCP Parameters]ip address:" << address
                                  << "port" << port;
        modbus_device_ = CreateTcpDevice(type, address, port);
    }

    if (!modbus_device_) {
        ui_->open_button_->setEnabled(true);
        qCWarning(logging_category_) << "Can not create modbus devices!";
        return;
    }

    if (IsServer(modbus_device_)) {
        if (!ResetServerMap(modbus_device_)) {
            qCWarning(logging_category_) << "Can not reset server map!";
            return;
        }

        bool is_busy = ui_->is_busy_->isChecked();
        bool listen_only = ui_->just_listen_->isChecked();
        int address = ui_->server_address->value();
        qCInfo(logging_category_) << "[TCP Parameters]is busy:" << is_busy
                                  << "listen_only" << listen_only
                                  << "server address:" << address;
        SetServerParameters(modbus_device_, QModbusServer::DeviceBusy,
                            is_busy);
        SetServerParameters(modbus_device_, QModbusServer::ListenOnlyMode,
                            listen_only);

        QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
        server->setServerAddress(address);
        SynchronizationRegister(server);
        connect(server, &QModbusServer::dataWritten,
                this, &SAKModbusUi::OnDateWritten);
    }

    if (IsClient(modbus_device_)) {
        ClientUpdateParameters();
    }

    connect(modbus_device_, &QModbusDevice::errorOccurred,
            this, &SAKModbusUi::OnErrorOccurred);
    bool opened = OpenDevice(modbus_device_);
    UpdateUiState(opened);

    if (!opened) {
        QString errStr = ModbuseDeviceErrorString(modbus_device_);
        ShowMessageBox(tr("Can not open device"),
                       tr("Can not open device: %1."
                          " Please check the parameters and try again!")
                           .arg(errStr));
    }
}

void SAKModbusUi::OnAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.address,
                        ui_->address_combo_box->currentText());
}

void SAKModbusUi::OnPortChanged() {
    settings_->setValue(mSettingsKeyCtx.port,
                        ui_->port_spin_box->value());
}

void SAKModbusUi::OnPortNameChanged() {
    settings_->setValue(mSettingsKeyCtx.portName,
                        ui_->port_name_->currentText());
}

void SAKModbusUi::OnParityChanged() {
    settings_->setValue(mSettingsKeyCtx.parity,
                        ui_->parity_->currentIndex());
}

void SAKModbusUi::OnBaudRateChanged() {
    settings_->setValue(mSettingsKeyCtx.baudRate,
                        ui_->baud_rate_->currentText());
}

void SAKModbusUi::OnDataBitsChanged() {
    settings_->setValue(mSettingsKeyCtx.dataBits,
                        ui_->data_bits_->currentIndex());
}

void SAKModbusUi::OnStopBistChanged() {
    settings_->setValue(mSettingsKeyCtx.stopBits,
                        ui_->stop_bits_->currentIndex());
}

void SAKModbusUi::OnInvokeRefresh() {
    InitComponentPortName();
}

void SAKModbusUi::OnClientTimeoutChanged() {
    settings_->setValue(mSettingsKeyCtx.clientTimeout,
                        ui_->timeout_->value());
    ClientUpdateParameters();
}

void SAKModbusUi::OnClientRepeatTimeChanged() {
    settings_->setValue(mSettingsKeyCtx.clientRepeatTime,
                        ui_->repeat_time_->value());
    ClientUpdateParameters();
}

void SAKModbusUi::OnServerIsBusyChanged() {
    settings_->setValue(mSettingsKeyCtx.serverIsBusy,
                        ui_->is_busy_->isChecked());
    if (IsServer(modbus_device_)) {
        SetServerParameters(modbus_device_, QModbusServer::DeviceBusy,
                                    ui_->is_busy_->isChecked());
    }
}

void SAKModbusUi::OnServerJustListenChanged() {
    settings_->setValue(mSettingsKeyCtx.serverJustListen,
                        ui_->just_listen_->isChecked());
    SetServerParameters(modbus_device_, QModbusServer::ListenOnlyMode,
                        ui_->just_listen_->isChecked());
}

void SAKModbusUi::OnServerAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.serverAddress,
                        ui_->server_address->value());
    if (IsServer(modbus_device_)) {
        QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
        server->setServerAddress(ui_->server_address->value());
    }
}

void SAKModbusUi::OnFunctionCodeChanged() {
    settings_->setValue(mSettingsKeyCtx.functionCode,
                        ui_->function_code_->currentIndex());
    ClientUpdateRWBtState();
}

void SAKModbusUi::OnTargetAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.targetAddress,
                        ui_->device_address_->value());
}

void SAKModbusUi::OnStartAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.startAddress,
                        ui_->start_address_->value());
    ClientUpdateTable();
}

void SAKModbusUi::OnAddressNumberChanged() {
    settings_->setValue(mSettingsKeyCtx.addressNumber,
                        ui_->quantity_->value());
    ClientUpdateTable();
}

void SAKModbusUi::OnReadClicked()
{
    if (!IsReady() || !IsClient(modbus_device_)) {
        return;
    }

    int register_type = ui_->function_code_->currentData().toInt();
    quint16 start_address = ui_->start_address_->value();
    quint16 quantity = ui_->quantity_->value();
    quint16 server_address = ui_->device_address_->value();
    quint8 function_code = ClientFunctionCode();

    qCInfo(logging_category_) << "[SendReadRequest]"
                              << "register type:" << register_type
                              << "start address:" << start_address
                              << "quantity:" << quantity
                              << "server address:" << server_address;

    typedef QModbusDataUnit::RegisterType RegisterType;
    RegisterType type = static_cast<RegisterType>(register_type);
#if 1
    QList<quint16> values;
    for (int i = 0; i < quantity; i++) {
        values.append(0);
    }
    QModbusDataUnit data_unit(type, start_address, values);
#else
    QModbusDataUnit data_unit(type, start_address, quantity);
#endif
    QModbusClient *client = qobject_cast<QModbusClient*>(modbus_device_);
    QModbusReply *reply = client->sendReadRequest(data_unit, server_address);
    if (!IsValidModbusReply(QVariant::fromValue(reply))) {
        return;
    }

    QByteArray tx = ReadRequestApplicationDataUnit(server_address,
                                                   function_code,
                                                   start_address,
                                                   quantity);
    QString info = QString("%1:%2").arg(ui_->function_code_->currentText(),
                                        QString::fromLatin1(tx.toHex(' ')));
    OutputMessage(info, false, TXCOLOR, TXFLAG);

    connect(reply, &QModbusReply::finished, this, [=](){
        OutputModbusReply(reply, function_code);
        if (reply->error() == QModbusDevice::NoError) {
            QJsonArray result = ModbusReplyResult(reply);
            ClientSetRegisterValue(result);
            reply->deleteLater();
        }
    });
}

void SAKModbusUi::OnWriteClicked() {
    if (!IsReady()) {
        return;
    }

    int registerType = ui_->function_code_->currentData().toInt();
    int start_address = ui_->start_address_->value();
    int server_address = ui_->device_address_->value();
    int quantity = ui_->quantity_->value();
    quint8 function_code = this->ClientFunctionCode();
    QJsonArray values = ClientRegisterValue();
    QModbusReply *reply = SendWriteRequest(modbus_device_, registerType,
                                           start_address, values,
                                           server_address);
    if (IsValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = QString("(to device:%1, function code:%2)")
                           .arg(server_address).arg(function_code);
        OutputMessage(info, false, TXCOLOR, TXFLAG);

        info = "";
        for (int i = start_address; i < values.count(); i++) {
            QString address = QString("%1").arg(QString::number(i), 5, '0');
            int temp = values.at(i - start_address).toInt();
            QString value_str = QString::number(temp, 16);
            QString value = QString("%1").arg(value_str, 4, '0');
            info += QString("%1:%2").arg(address, value);

            if (i != (i + start_address + values.count() - 1)) {
                info.append("\n");
            }
        }

        QByteArray tx = WriteRequestApplicationDataUnit(server_address,
                                                        function_code,
                                                        start_address,
                                                        quantity,
                                                        values);
        OutputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                      TXFLAG + QString::number(server_address));
    }
}

void SAKModbusUi::OnSendClicked() {
    if (!IsReady()) {
        return;
    }

    quint8 server_address = ui_->device_address_->value();
    QByteArray pdu = ClientPdu();
    QByteArray data = pdu.length()
                          ? QByteArray(pdu.data() + 1, pdu.length() - 1)
                          : QByteArray();
    int functionCode = pdu.count() ? pdu.at(0) : int(QModbusDataUnit::Invalid);
    QModbusReply *reply = SendRawRequest(
        modbus_device_, server_address, functionCode, data);

    qCWarning(logging_category_) << "address:" << server_address
                                 << "function code:" << functionCode
                                 << "data:" << QString(pdu.toHex(' '));

    if (IsValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, functionCode);
            reply->deleteLater();
        });

        OutputModbusRequestSend(server_address, pdu);
    }

    // Update settings data.
    int index = settings_->value(mSettingsKeyCtx.sendHistoryIndex).toInt();
    bool ret = WriteSettingsArray(mSettingsKeyCtx.sendHistory,
                                  mSettingsKeyCtx.pdu,
                                  QString(pdu.toHex(' ')),
                                  index,
                                  MAX_HISTORY_INDEX);
    if (ret) {
        if (index > ui_->pdu_->count()) {
            ui_->pdu_->addItem(QString(pdu.toHex(' ')));
        } else {
            ui_->pdu_->insertItem(index, QString(pdu.toHex(' ')));
        }
        settings_->setValue(mSettingsKeyCtx.sendHistoryIndex,
                            index + 1 > MAX_HISTORY_INDEX ? 0 : index + 1);
    }
}

void SAKModbusUi::OnDateWritten(QModbusDataUnit::RegisterType table,
                                int address, int size) {
    qCInfo(logging_category_) << "Data written, table:" << table
                              << ", start address:" << address
                              << ", size:" << size << ".";
    QTableView *tv = TableView(RegisterType(table));
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
    QJsonArray data = ServerValues(server, table, address, size);
    size = qMin<int>(data.count(), size);
    for (int i = 0; i < size; i++) {
        int row = address + i;
        int base = 16;
        int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
        int value = data.at(i).toInt();
        QString cookedStr = QString::number(value, base)
                                .rightJustified(width, '0', true);
        QStandardItem *itemTemp = model->item(row, 1);
        if (itemTemp) {
            itemTemp->setData(cookedStr, Qt::DisplayRole);
            itemTemp->setTextAlignment(Qt::AlignCenter);
        }
    }

    tv->viewport()->update();
}


void SAKModbusUi::OnItemChanged(QStandardItem *item) {
    if (!item) {
        return;
    }

    if (item->column() != 1) {
        return;
    }

    if (IsServer(modbus_device_)) {
        int address = item->row();
        int currentIndex = ui_->server_registers_->currentIndex();
        QModbusDataUnit::RegisterType table = QModbusDataUnit::Invalid;
        if (currentIndex == 0) {
            table = QModbusDataUnit::DiscreteInputs;
        } else if (currentIndex == 1) {
            table = QModbusDataUnit::Coils;
        }  else if (currentIndex == 2) {
            table = QModbusDataUnit::InputRegisters;
        }  else if (currentIndex == 3) {
            table = QModbusDataUnit::HoldingRegisters;
        } else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow table type!");
        }

        quint16 value = item->text().toInt(Q_NULLPTR, 16);
        SetServerData(modbus_device_, table, address, value);
    }
}

void SAKModbusUi::UpdateUiState(bool opened) {
    ui_->device_list_->setEnabled(!opened);
    ui_->cloese_button_->setEnabled(opened);
    ui_->open_button_->setEnabled(!opened);
    ui_->networkGroupBox->setEnabled(!opened);
    ui_->serialPortGroupBox->setEnabled(!opened);
#if 0
    ui->clientGroupBox->setEnabled(!opened);
    ui->serverGroupBox->setEnabled(!opened);
#endif
}

quint16 SAKModbusUi::CalculateModbusCrc(const QByteArray &data) {
    int len = data.size();
    uint16_t crc = 0XFFFF;
    uint8_t temp;

    for (int i = 0; i < len; i++) {
       temp = data.at(i);
       crc ^= temp;
       for (int j = 0; j < 8; j++){
          if ( crc & 0X0001){
              crc >>= 1;
              crc ^= 0XA001;
          } else {
              crc >>= 1;
          }
       }
    }

    return crc;
}

quint16 SAKModbusUi::CookedRegisterString(QString text, int base) {
    QRegularExpression re;
    if (base == 2) { // Retain [01]
        re = QRegularExpression("[ -/2-~]");
    } else if (base == 10) { // Retain [0-9]
        re = QRegularExpression("[ -/:-~]");
    } else { // Retain [0-9A-Fa-f]
        re = QRegularExpression("[ -/:-@G-`g-~]");
    }

    while (text.contains(re)) {
        text = text.remove(re);
    }

    int value = 0;
    if (base == 2) {
        text = text.rightJustified(16, '0', true);
        value = text.toInt(Q_NULLPTR, 2);
    } else if (base == 10) {
        if (text.toInt() > UINT16_MAX || text.toInt() < 0) {
            text = QString::number(UINT16_MAX);
        }

        value = text.toInt(Q_NULLPTR, 10);
    } else {
        text = text.rightJustified(4, '0', true);
        value = text.toInt(Q_NULLPTR, 16);
    }

    return value;
}

void SAKModbusUi::SynchronizationRegister(QModbusDevice *server) {
    for (int i = 0; i < 4; i++) {
        QWidget *widget = ui_->server_registers_->widget(i);
        QTableView *tv = qobject_cast<QTableView*>(widget);
        auto *model = reinterpret_cast<QStandardItemModel*>(tv->model());
        for (int row = 0; row < model->rowCount(); row++) {
            QStandardItem *item = model->item(row, 3);
            if (item) {
                int type = QModbusDataUnit::Invalid;
                if (i == 0) {
                    type = QModbusDataUnit::DiscreteInputs;
                } else if (i == 1) {
                    type = QModbusDataUnit::Coils;
                } else if (i == 2) {
                    type = QModbusDataUnit::InputRegisters;
                }  else if (i == 3) {
                    type = QModbusDataUnit::HoldingRegisters;
                } else {
                    type = QModbusDataUnit::Invalid;
                }

                quint16 value = item->text().toInt(Q_NULLPTR, 16);
                QModbusDataUnit::RegisterType table =
                    static_cast<QModbusDataUnit::RegisterType>(type);
                if (!SetServerData(server, table, row, value)) {
                    qCWarning(logging_category_) << "Set server data error!";
                }
            }
        }
    }
}

bool SAKModbusUi::WriteSettingsArray(const QString &group, const QString &key,
                                     const QString &value, int index,
                                     int max_index) {
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

QModbusDataUnit::RegisterType SAKModbusUi::RegisterType(int type) {
    if (type == QModbusDataUnit::DiscreteInputs) {
        return QModbusDataUnit::DiscreteInputs;
    } else if (type == QModbusDataUnit::Coils) {
        return QModbusDataUnit::Coils;
    } else if (type == QModbusDataUnit::InputRegisters) {
        return QModbusDataUnit::InputRegisters;
    } else if (type == QModbusDataUnit::HoldingRegisters) {
        return QModbusDataUnit::HoldingRegisters;
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Invalid register type!");
        return QModbusDataUnit::Invalid;
    }
}

void SAKModbusUi::ClientUpdateTable() {
    int number = ui_->quantity_->value();
    int rowCount = register_model_->rowCount();
    if (number > rowCount) {
        register_model_->insertRows(rowCount, number - rowCount);
    } else if (number < rowCount) {
        register_model_->removeRows(number, rowCount - number);
    }

    // Update address.
    int startAddress = ui_->start_address_->value();
    TableViewUpdateAddress(ui_->client_registers_, startAddress);
}

void SAKModbusUi::ClientSetRegisterValue(const QJsonArray &values) {
    for (int row = 0; row < values.count(); row++) {
        int value = values.at(row).toInt();
        QModelIndex index = register_model_->index(row, 1);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, QString::number(value));
        register_model_->setItemData(index, roles);
    }

    // Refresh the view, or the new value will not be show.
    ui_->client_registers_->viewport()->update();
}

void SAKModbusUi::ClientUpdateRWBtState() {
    QStringList list = ui_->function_code_->currentText().split('-');
    int code = list.length() ? list.first().toInt(Q_NULLPTR, 16) : 0;
    bool is_reading_operation = false;
    if (code == 0x01 || code == 0x02 || code == 0x03 || code == 0x04) {
        is_reading_operation = true;
    }

    if (code == 0x05 || code == 0x06) {
        ui_->quantity_->setValue(1);
    }

    ui_->read_->setEnabled(is_reading_operation);
    ui_->write_->setEnabled(!is_reading_operation);
}

void SAKModbusUi::ClientUpdateParameters() {
    if (IsClient(modbus_device_)) {
        int timeout = ui_->timeout_->value();
        int repeat_time = ui_->repeat_time_->value();
        SetClientParameters(modbus_device_, timeout, repeat_time);
    }
}

quint8 SAKModbusUi::ClientFunctionCode() {
    QString txt = ui_->function_code_->currentText();
    qDebug() << txt;
    QStringList list = txt.split('-', Qt::SkipEmptyParts);
    if (list.length()) {
        return list.first().toInt(Q_NULLPTR, 16);
    }

    return 0;
}

QJsonArray SAKModbusUi::ClientRegisterValue() {
    QJsonArray array;
    for (int row = 0; row < register_model_->rowCount(); row++) {
        QStandardItem *item = register_model_->item(row, 1);
        if (item) {
            QString text = item->text();
            array.append(text.toInt(Q_NULLPTR, 16));
        } else {
            array.append(0);
        }
    }

    return array;
}

QByteArray SAKModbusUi::ClientPdu() {
    QString text = ui_->pdu_->currentText();
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

void SAKModbusUi::ShowMessageBox(const QString &title, const QString &msg) {
    QMessageBox::warning(this, title, msg);
}

void SAKModbusUi::ShowMessageBoxDeviceIsNotReady() {
    ShowMessageBox(tr("Device is not ready"),
                   tr("The device is not ready,"
                      " please open the device then try again."));
}

void SAKModbusUi::OutputModbusReply(QModbusReply *reply, int function_code) {
    if (reply->error() != QModbusDevice::NoError) {
        OutputMessage(reply->errorString(), true, "", "error");
        QMessageBox::warning(this, tr("Error occured"), reply->errorString());
        return;
    }

    int server_address = reply->serverAddress();
    int start_address = reply->result().startAddress();
    QList<quint16> values = reply->result().values();

    if (reply->type() == QModbusReply::ReplyType::Raw) {
        QByteArray data = reply->rawResult().data();
        QString info = QString("(from address:%1, "
                               "function code:%2), "
                               "data unit:%3")
                           .arg(server_address)
                           .arg(function_code)
                           .arg(QString::fromLatin1(data.toHex(' ')));
        OutputMessage(info, false, RXCOLOR, RXFLAG);
    } else if (reply->type() == QModbusReply::ReplyType::Common) {
#if 0
        QByteArray raw_data_unit;
        raw_data_unit.append(char(server_address));
        quint16 be_function_code = function_code;
        be_function_code = qToBigEndian<quint16>(be_function_code);
        raw_data_unit.append((char*)(&be_function_code), 2);
        quint16 be_value_count = values.count();
        be_value_count = qToBigEndian<quint16>(be_value_count);
        raw_data_unit.append((char*)(&be_value_count), 2);
        for (int i = 0; i < values.count(); i++) {
            quint16 value = values.at(i);
            quint16 big_endian_value = qToBigEndian<quint16>(value);
            raw_data_unit.append((char*)(&big_endian_value), 2);
        }
        quint16 crc = CalculateModbusCrc(raw_data_unit);
        raw_data_unit.append((char*)(&crc), 2);
        QString info = QString("%1: %2")
                           .arg(ui_->function_code_->currentText(),
                                QString::fromLatin1(raw_data_unit.toHex(' ')));
        OutputMessage(info, false, RXCOLOR, RXFLAG);
#endif
        info = "";
        for (int i = 0; i < values.count(); i++) {
            QString address = QString::number(i + start_address);
            address = QString("%1").arg(address, 5, '0');

            int index = i - start_address;
            QString value_str = QString::number(values.at(index), 16);
            QString value = QString("%1").arg(value_str, 4, '0');
            info += QString("%1:%2").arg(address, value);

            if (i != (values.count() - 1)) {
                info.append("\n");
            }
        }
        ui_->text_browser_->append(info);
    }
}

void SAKModbusUi::OutputModbusRequestSend(int server_address,
                                        const QByteArray &pdu) {
    QByteArray tx;
    tx.append(char(server_address));
    tx.append(pdu);
    quint16 crc = CalculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    OutputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(server_address));
}

QByteArray SAKModbusUi::ReadRequestApplicationDataUnit(int server_address,
                                                       int function_code,
                                                       int start_address,
                                                       int quantity) {
    QByteArray tx;
    quint16 big_endian_start_address = qToBigEndian<quint16>(start_address);
    quint16 big_endian_quantity = qToBigEndian<quint16>(quantity);
    tx.append(char(server_address));
    tx.append(char(function_code));
    tx.append(reinterpret_cast<char*>(&big_endian_start_address), 2);
    tx.append(reinterpret_cast<char*>(&big_endian_quantity), 2);
    quint16 crc = CalculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    return tx;
}

QByteArray SAKModbusUi::WriteRequestApplicationDataUnit(int server_address,
                                                        int function_code,
                                                        int start_address,
                                                        int quantity,
                                                        QJsonArray values) {
    QByteArray bytes;
    for (int i = 0; i < values.count(); i++) {
        quint16 value = values.at(i).toInt();
        quint16 big_endian_value = qToBigEndian<quint16>(value);
        bytes.append(reinterpret_cast<char*>(&big_endian_value), 2);
    }

    QByteArray tx;
    quint16 big_endian_start_address = qToBigEndian<quint16>(start_address);
    quint16 big_endian_quantity = qToBigEndian<quint16>(quantity);
    tx.append(char(server_address));
    tx.append(char(function_code));
    tx.append(reinterpret_cast<char*>(&big_endian_start_address), 2);
    tx.append(reinterpret_cast<char*>(&big_endian_quantity), 2);
    tx.append(bytes);
    quint16 crc = CalculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    return tx;
}

void SAKModbusUi::OutputMessage(const QString &msg, bool isError,
                                const QString &color, const QString &flag) {
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

    cookedMsg += flag.isEmpty()
            ? ""
            : QString("<font color=%1>[%2]</font> ").arg(cookedColor, flag);
    cookedMsg += msg;
    ui_->text_browser_->append(cookedMsg);
}

QTableView *SAKModbusUi::TableView(QModbusDataUnit::RegisterType table) {
    QWidget *tv = Q_NULLPTR;
    if (table == QModbusDataUnit::DiscreteInputs) {
        tv = ui_->server_registers_->widget(0);
    } else if (table == QModbusDataUnit::Coils) {
        tv = ui_->server_registers_->widget(1);
    } else if (table == QModbusDataUnit::InputRegisters) {
        tv = ui_->server_registers_->widget(2);
    } else if (table == QModbusDataUnit::HoldingRegisters) {
        tv = ui_->server_registers_->widget(3);
    } else {
        tv = ui_->server_registers_->widget(3);
        qCWarning(logging_category_)
                << "Get table view failed: unknow register type!";
    }

    return qobject_cast<QTableView*>(tv);
}

QTableView *SAKModbusUi::TableViewInit(int rowCount, QTableView *tv) {
    if (!tv) {
        tv = new QTableView(this);
    }

    QHeaderView *hv = tv->horizontalHeader();
    QStandardItemModel *model = new QStandardItemModel(tv);
    QStringList labels =
        QStringList() << tr("Address") << tr("Value") << tr("Description");
    model->setHorizontalHeaderLabels(labels);
    model->setColumnCount(3);
    model->setRowCount(rowCount);
    tv->setModel(model);
    tv->verticalHeader()->hide();
    tv->setItemDelegateForColumn(0, new ReadOnlyDelegate(tv));
    TableViewUpdateAddress(tv, 0);
    hv->setStretchLastSection(true);

#if 1
    // Set the default value to 0.
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = model->index(row, 1);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, "0");
        model->setItemData(index, roles);

        QStandardItem *item = model->item(row, 3);
        OnItemChanged(item);
    }
#endif

    connect(model, &QStandardItemModel::itemChanged,
            this, &SAKModbusUi::OnItemChanged);

    return tv;
}

QVector<quint16> SAKModbusUi::TableValues(QTableView *tv, int row, int count) {
    if (!tv) {
        qCWarning(logging_category_) << "Table view can not be null!";
        return QVector<quint16>();
    }

    auto *model = qobject_cast<QStandardItemModel*>(tv->model());
    if (!model) {
        qCWarning(logging_category_) << "Model can not be null!";
        return QVector<quint16>();
    }

    QVector<quint16> values;
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

void SAKModbusUi::TableViewUpdateAddress(QTableView *tv, int startAddress) {
    QStandardItemModel *model =
            reinterpret_cast<QStandardItemModel*>(tv->model());
    int number = model->rowCount();
    for (int address = startAddress, row = 0; row < number; address++, row++) {
        QString text = QString("%1").arg(QString::number(address), 5, '0');
        QModelIndex index = model->index(row, 0);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, text);
        model->setItemData(index, roles);

        QStandardItem *itemTemp = model->item(row, 0);
        if (itemTemp) {
            itemTemp->setTextAlignment(Qt::AlignCenter);
        }
    }
}

QModbusDevice *SAKModbusUi::CreateRtuSerialDevice(
        int deviceType, const QString &portName, int parity, int baudRate,
        int dataBits, int stopBits)
{
    QModbusDevice *device = Q_NULLPTR;
    if (deviceType == ModbusDeviceRtuSerialClient) {
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        device = new QModbusRtuSerialMaster(this);
#else
        device = new QModbusRtuSerialClient(this);
#endif
    } else if (deviceType == ModbusDeviceRtuSerialServer) {
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        device = new QModbusRtuSerialSlave(this);
#else
        device = new QModbusRtuSerialServer(this);
#endif
    }

    if (device) {
        device->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                       portName);
        device->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                       parity);
        device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                       baudRate);
        device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                       dataBits);
        device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                       stopBits);
        return device;
    }

    Q_ASSERT_X(device, __FUNCTION__, "Unknow device type.");
    return Q_NULLPTR;
}

QModbusDevice *SAKModbusUi::CreateTcpDevice(
        int deviceType, QString address, int port) {
    QModbusDevice *device = Q_NULLPTR;
    if (deviceType == ModbusDeviceTcpClient) {
        device = new QModbusTcpClient(this);
    } else if (deviceType == ModbusDeviceTcpServer) {
        device = new QModbusTcpServer(this);
    }

    if (device) {
        device->setConnectionParameter(QModbusDevice::NetworkAddressParameter,
                                       address);
        device->setConnectionParameter(QModbusDevice::NetworkPortParameter,
                                       port);
        return device;
    }

    return Q_NULLPTR;
}

bool SAKModbusUi::IsClient(QModbusDevice *device) {
    if (device && qobject_cast<QModbusClient*>(device)) {
        return true;
    }

    qCWarning(logging_category_) << "Not a modbus client!";
    return false;
}

bool SAKModbusUi::IsServer(QModbusDevice *device)
{
    if (device && qobject_cast<QModbusServer*>(device)) {
        return true;
    }

    return false;
}

bool SAKModbusUi::ResetServerMap(QModbusDevice *server) {
    if (server) {
        QVector<quint16> values(UINT16_MAX + 1, 0);
        QModbusDataUnit dataUnit(QModbusDataUnit::Coils, 0, values);

        QModbusDataUnitMap map;
        map.insert(QModbusDataUnit::Coils, dataUnit);
        map.insert(QModbusDataUnit::DiscreteInputs, dataUnit);
        map.insert(QModbusDataUnit::HoldingRegisters, dataUnit);
        map.insert(QModbusDataUnit::InputRegisters, dataUnit);

        if (server->inherits("QModbusServer")) {
            QModbusServer *modbusServer =
                    reinterpret_cast<QModbusServer*>(server);
            return modbusServer->setMap(map);
        }
    } else {
        qWarning(logging_category_) << "Not a modbus server!";
        return false;
    }

    return false;
}


bool SAKModbusUi::SetClientParameters(
        QModbusDevice *device, int timeout, int numberOfRetries) {
    if (device) {
        if (device->inherits("QModbusClient")) {
            QModbusClient *modbusClient = qobject_cast<QModbusClient*>(device);
            modbusClient->setTimeout(timeout);
            modbusClient->setNumberOfRetries(numberOfRetries);
            return true;
        }
    }

    return false;
}

bool SAKModbusUi::SetServerParameters(
        QModbusDevice *device, int option, QVariant value) {
    if (device) {
        if (device->inherits("QModbusServer")) {
            QModbusServer *modbusServer = qobject_cast<QModbusServer*>(device);
            modbusServer->setValue(option, value);
            return true;
        }
    }

    return false;
}

bool SAKModbusUi::OpenDevice(QModbusDevice *device) {
    if (device) {
        if (device->inherits("QModbusDevice")) {
            return device->connectDevice();
        }
    }
    return false;
}

QString SAKModbusUi::ModbuseDeviceErrorString(QModbusDevice *device) {
    if (device) {
        if (device->inherits("QModbusDevice")) {
            return device->errorString();
        } else {
            return QString("Invalid device object!");
        }
    }

    return QString("Invalid modbus device!");
}

bool SAKModbusUi::SetServerData(QModbusDevice *server,
                                QModbusDataUnit::RegisterType table,
                                quint16 address, quint16 data) {
    if (server && qobject_cast<QModbusServer*>(server)) {
        QModbusServer *modbusServer = qobject_cast<QModbusServer*>(server);
        qCInfo(logging_category_) << "[Set table value]table:" << table
                                  << "address:" << address
                                  << "data:" << data;
        return modbusServer->setData(table, address, data);
    }

    return false;
}

QJsonArray SAKModbusUi::ServerValues(QModbusServer *server, int registerType,
                                     int address, int size) {
    QJsonArray values;
    if (server) {
        typedef QModbusDataUnit::RegisterType Table;
        Table table = static_cast<Table>(registerType);
        for (int i = address; i < size; i++) {
            quint16 value;
            if (server->data(table, i, &value)) {
                values.append(value);
            } else {
                qCWarning(logging_category_) << "Parameters error!";
                break;
            }
        }
    } else {
        qCWarning(logging_category_) << "Can not get values from null object!";
    }

    return values;
}

QModbusReply *SAKModbusUi::SendWriteRequest(QModbusDevice *device,
                                            int registerType,
                                            int startAddress,
                                            QJsonArray values,
                                            int serverAddress) {
    if (device && IsClient(device)) {
        typedef QModbusDataUnit::RegisterType Type;
        Type cookedRegisterType = static_cast<Type>(registerType);
        QVector<quint16> registerValues;
        for (int i = 0; i < values.count(); i++) {
            registerValues.append(values.at(i).toInt());
        }

        QModbusDataUnit dataUnit(cookedRegisterType,
                                 startAddress, registerValues);
        if (dataUnit.isValid()) {
            qCInfo(logging_category_) << "register-type:" << registerType
                                     << " start-address:" << startAddress
                                     << " serverAddress:" << serverAddress
                                     << " values:" << values;

            QModbusClient *client = qobject_cast<QModbusClient*>(device);
            QModbusReply *reply = client->sendWriteRequest(dataUnit,
                                                           serverAddress);
            return reply;
        } else {
            qCWarning(logging_category_) << "Unvalid data unit!";
        }
    }

    return Q_NULLPTR;
}

bool SAKModbusUi::IsValidModbusReply(QVariant reply) {
    QModbusReply *modbusReply = reply.value<QModbusReply*>();
    if (!modbusReply) {
        qCWarning(logging_category_) << "QModbusReply is null!";
        return false;
    } else if (modbusReply->isFinished()) {
        qCWarning(logging_category_) << "QModbusReply is finished!";
        return false;
    } else {
        return true;
    }

    return false;
}

QJsonArray SAKModbusUi::ModbusReplyResult(QModbusReply *reply) {
    if (!reply) {
        return QJsonArray();
    }

    if (!reply->inherits("QModbusReply")) {
        return QJsonArray();
    }

    QJsonArray resultJsonArray;
    if (reply->type() == QModbusReply::Common) {
        QModbusDataUnit dataUnit = reply->result();
        for (uint i = 0; i < dataUnit.valueCount(); i++) {
            quint16 rawData = dataUnit.value(i);
            int registerType = dataUnit.registerType();
            int coils = QModbusDataUnit::Coils;
            int discreteInputs = QModbusDataUnit::DiscreteInputs;
            if ((registerType == coils) || (registerType == discreteInputs)) {
                if (rawData != 0) {
                    rawData = 1;
                }
            }
            resultJsonArray.append(int(rawData));
        }

        return resultJsonArray;
    }

    QModbusResponse rawResult = reply->rawResult();
    QByteArray data = rawResult.data();
    for (int i = 0; i < data.length(); i++) {
        resultJsonArray.append(data.at(i));
    }

    return resultJsonArray;
}

QModbusReply *SAKModbusUi::SendRawRequest(
        QModbusDevice *device, int serverAddress, int functionCode,
        const QByteArray &data) {
    auto fc = static_cast<QModbusPdu::FunctionCode>(functionCode);
    QModbusRequest request(fc, data);
    if (device && IsClient(device)) {
        QModbusClient *client = qobject_cast<QModbusClient*>(device);
        return client->sendRawRequest(request, serverAddress);
    }

    return Q_NULLPTR;
}

bool SAKModbusUi::IsReady() {
    if (modbus_device_) {
        if (modbus_device_->state() == QModbusDevice::ConnectedState) {
            return true;
        }
    }

    QMessageBox::warning(this,
                         tr("Device is not Ready"),
                         tr("The modbus device is not ready, "
                            "please check settings and try again later!"));

    return false;
}

QByteArray SAKModbusUi::ApplicationDataUnit(const QModbusRequest &request,
                                            bool is_tcp) {
    Q_UNUSED(request)
    Q_UNUSED(is_tcp)
    return QByteArray();
}
