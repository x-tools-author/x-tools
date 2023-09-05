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

struct SAKModbusUiSettingKeys {
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
    const QString client_repeat_time= "SAKModbus/clientRepeatTime";

    const QString server_is_busy = "SAKModbus/serverIsBusy";
    const QString server_just_listen= "SAKModbus/serverJustListen";
    const QString server_address= "SAKModbus/serverAddress";

    const QString function_code = "SAKModbus/functionCode";
    const QString target_address = "SAKModbus/targetAddress";
    const QString start_address = "SAKModbus/startAddress";
    const QString address_number = "SAKModbus/addressNumber";

    const QString send_history = "SAKModbus/sendHistory";
    const QString send_history_index = "SAKModbus/sendHistoryIndex";
    const QString pdu = "pdu";
};

class ReadOnlyDelegate: public QItemDelegate {
  public:
    ReadOnlyDelegate(QWidget *parent = Q_NULLPTR) : QItemDelegate(parent) {

    }
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
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
    , key_ctx_(new SAKModbusUiSettingKeys){
    if (!settings_) {
        settings_ = SAKSettings::instance();
    }

    ui_->setupUi(this);

    InitComponents();
    InitSettings();
    InitSignals();

    UpdateUiState(false);
    OnDeviceTypeChanged();
    UpdateClientTableView();
    UpdateClientReadWriteButtonState();
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
    ui_->device_list_->addItem(tr("RtuClient"),
                               SAKModbusFactory::kModbusRtuSerialClient);
    ui_->device_list_->addItem(tr("RtuServer"),
                               SAKModbusFactory::kModbusRtuSerialServer);
    ui_->device_list_->addItem(tr("TcpClient"),
                               SAKModbusFactory::kModbusTcpClient);
    ui_->device_list_->addItem(tr("TcpServer"),
                               SAKModbusFactory::kModbusTcpServer);
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
    QTableView *table_view = CreateTableView(1, ui_->client_registers_);
    register_model_ = qobject_cast<QStandardItemModel*>(table_view->model());
}

void SAKModbusUi::InitComponentInput() {
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui_->pdu_->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void SAKModbusUi::InitComponentRegisterTabWidget() {
    QTabWidget *tab_widget = ui_->server_registers_;
    QStringList titles = QStringList() << tr("Coils")
                                       << tr("DiscreteInputs")
                                       << tr("InputRegisters")
                                       << tr("HoldingRegisters");
    for (QString &title : titles) {
        QTableView *table_view = CreateTableView(UINT16_MAX + 1, Q_NULLPTR);
        table_view->verticalHeader()->hide();
        tab_widget->addTab(table_view, title);
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
    int deviceIndex = settings_->value(key_ctx_->device_index).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui_->device_list_->count()) {
        ui_->device_list_->setCurrentIndex(deviceIndex);
    }
}

void SAKModbusUi::InitSettingsNetwork() {
    QString address = settings_->value(key_ctx_->address).toString();
    ui_->address_combo_box->setCurrentText(address);

    QVariant portValiant = settings_->value(key_ctx_->port);
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

    QString portName = settings_->value(key_ctx_->port_name).toString();
    int index = ui_->port_name_->findText(portName);
    SetComboBoxIndex(index, ui_->port_name_);

    index = settings_->value(key_ctx_->parity).toInt();
    SetComboBoxIndex(index, ui_->parity_);

    QString bd = settings_->value(key_ctx_->baud_rate).toString();
    ui_->baud_rate_->setCurrentText(bd);

    index = settings_->value(key_ctx_->data_bits).toInt();
    SetComboBoxIndex(index, ui_->data_bits_);

    index = settings_->value(key_ctx_->stop_bits).toInt();
    SetComboBoxIndex(index, ui_->stop_bits_);
}

void SAKModbusUi::InitSettingsClient() {
    int timeout = settings_->value(key_ctx_->client_timeout).toInt();
    ui_->timeout_->setValue(timeout < 100 ? 100 : timeout);

    const QString key = key_ctx_->client_repeat_time;
    int repeatTimes =settings_->value(key).toInt();
    ui_->repeat_time_->setValue(repeatTimes);
}

void SAKModbusUi::InitSettingsServer() {
    bool isBusy = settings_->value(key_ctx_->server_is_busy).toBool();
    ui_->is_busy_->setChecked(isBusy);

    QString key = key_ctx_->server_just_listen;
    bool just_listen = settings_->value(key).toBool();
    ui_->just_listen_->setChecked(just_listen);

    int address = settings_->value(key_ctx_->server_address).toInt();
    ui_->server_address->setValue(address);
}

void SAKModbusUi::InitSettingsClientOperations() {
    int index = settings_->value(key_ctx_->function_code).toInt();
    if (index >= 0 && index < ui_->function_code_->count()) {
        ui_->function_code_->setCurrentIndex(index);
    }

    int address = settings_->value(key_ctx_->target_address).toInt();
    ui_->device_address_->setValue(address);

    int start = settings_->value(key_ctx_->start_address).toInt();
    ui_->start_address_->setValue(start);

    int number = settings_->value(key_ctx_->address_number).toInt();
    ui_->quantity_->setValue(number);
}

void SAKModbusUi::InitSettingsInput() {
    ui_->pdu_->clear();
    settings_->beginReadArray(key_ctx_->send_history);
    for (int i = 0; i < 10; i++) {
        settings_->setArrayIndex(i);
        QString text = settings_->value(key_ctx_->pdu).toString();
        if (!text.isEmpty()) {
            ui_->pdu_->addItem(text);
        }
    }
    settings_->endArray();

    int index = settings_->value(key_ctx_->send_history_index).toInt();
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
            this, &SAKModbusUi::OnCloseClicked);
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
        OnCloseClicked();
        QMessageBox::warning(this, tr("Error occured"), msg);
    }
}

void SAKModbusUi::OnDeviceTypeChanged() {
    int type = ui_->device_list_->currentData().toInt();
    bool isSerial = (type == SAKModbusFactory::kModbusRtuSerialClient
                     || type == SAKModbusFactory::kModbusRtuSerialServer);
    bool isClient = (type == SAKModbusFactory::kModbusRtuSerialClient
                     || type == SAKModbusFactory::kModbusTcpClient);

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
        ui_->console_->setVisible(true);
        ui_->serverGroupBox->setHidden(true);
        ui_->registersGroupBox->setHidden(true);
        ui_->clientOperationsGroupBox->setHidden(false);
        ui_->clientCustomCommandGroupBox->setHidden(false);
        ui_->clientRegistersGroupBox->setHidden(false);
    } else {
        ui_->console_->setVisible(false);
        ui_->clientCustomCommandGroupBox->setHidden(true);
        ui_->clientRegistersGroupBox->setHidden(true);
        ui_->clientOperationsGroupBox->setHidden(true);
        ui_->serverGroupBox->setHidden(false);
        ui_->registersGroupBox->setHidden(false);
    }

    settings_->setValue(key_ctx_->device_index, type);
}

void SAKModbusUi::OnCloseClicked() {
    SAKModbusFactory::Instance()->DeleteModbusDevuce(&modbus_device_);
    UpdateUiState(false);
}

void SAKModbusUi::OnOpenClicked() {
    ui_->open_button_->setEnabled(false);
    SAKModbusFactory::Instance()->DeleteModbusDevuce(&modbus_device_);

    modbus_device_ = CreateModbusDevice();

    if (SAKModbusFactory::Instance()->IsServerDevice(modbus_device_)) {
        if (!UpdateServerMap(modbus_device_)) {
            ui_->open_button_->setEnabled(true);
            qCWarning(kLoggingCategory) << "Can not reset server map!";
            return;
        }

        UpdateServerParameters();

        QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
        UpdateServerRegistersData();
        connect(server, &QModbusServer::dataWritten,
                this, &SAKModbusUi::OnDateWritten);
    } else if (SAKModbusFactory::Instance()->IsClientDevice(modbus_device_)) {
        UpdateClientParameters();
    } else {
        ui_->open_button_->setEnabled(true);
        qCWarning(kLoggingCategory) << "Can not create modbus devices!";
        return;
    }

    connect(modbus_device_, &QModbusDevice::errorOccurred,
            this, &SAKModbusUi::OnErrorOccurred);
    SAKModbusFactory *factory = SAKModbusFactory::Instance();
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

void SAKModbusUi::OnAddressChanged() {
    settings_->setValue(key_ctx_->address,
                        ui_->address_combo_box->currentText());
}

void SAKModbusUi::OnPortChanged() {
    settings_->setValue(key_ctx_->port,
                        ui_->port_spin_box->value());
}

void SAKModbusUi::OnPortNameChanged() {
    settings_->setValue(key_ctx_->port_name,
                        ui_->port_name_->currentText());
}

void SAKModbusUi::OnParityChanged() {
    settings_->setValue(key_ctx_->parity,
                        ui_->parity_->currentIndex());
}

void SAKModbusUi::OnBaudRateChanged() {
    settings_->setValue(key_ctx_->baud_rate,
                        ui_->baud_rate_->currentText());
}

void SAKModbusUi::OnDataBitsChanged() {
    settings_->setValue(key_ctx_->data_bits,
                        ui_->data_bits_->currentIndex());
}

void SAKModbusUi::OnStopBistChanged() {
    settings_->setValue(key_ctx_->stop_bits,
                        ui_->stop_bits_->currentIndex());
}

void SAKModbusUi::OnInvokeRefresh() {
    InitComponentPortName();
}

void SAKModbusUi::OnClientTimeoutChanged() {
    settings_->setValue(key_ctx_->client_timeout,
                        ui_->timeout_->value());
    UpdateClientParameters();
}

void SAKModbusUi::OnClientRepeatTimeChanged() {
    settings_->setValue(key_ctx_->client_repeat_time,
                        ui_->repeat_time_->value());
    UpdateClientParameters();
}

void SAKModbusUi::OnServerIsBusyChanged() {
    settings_->setValue(key_ctx_->server_is_busy,
                        ui_->is_busy_->isChecked());
    UpdateServerParameters();
}

void SAKModbusUi::OnServerJustListenChanged() {
    settings_->setValue(key_ctx_->server_just_listen,
                        ui_->just_listen_->isChecked());
    UpdateServerParameters();
}

void SAKModbusUi::OnServerAddressChanged() {
    settings_->setValue(key_ctx_->server_address,
                        ui_->server_address->value());
    UpdateServerParameters();
}

void SAKModbusUi::OnFunctionCodeChanged() {
    settings_->setValue(key_ctx_->function_code,
                        ui_->function_code_->currentIndex());
    UpdateClientReadWriteButtonState();
}

void SAKModbusUi::OnTargetAddressChanged() {
    settings_->setValue(key_ctx_->target_address,
                        ui_->device_address_->value());
}

void SAKModbusUi::OnStartAddressChanged() {
    settings_->setValue(key_ctx_->start_address,
                        ui_->start_address_->value());
    UpdateClientTableView();
}

void SAKModbusUi::OnAddressNumberChanged() {
    settings_->setValue(key_ctx_->address_number,
                        ui_->quantity_->value());
    UpdateClientTableView();
}

void SAKModbusUi::OnReadClicked()
{
    if (!IsConnected()) {
        return;
    }

    if (!SAKModbusFactory::Instance()->IsClientDevice(modbus_device_)) {
        return;
    }

    int register_type = ui_->function_code_->currentData().toInt();
    quint16 start_address = ui_->start_address_->value();
    quint16 quantity = ui_->quantity_->value();
    quint16 server_address = ui_->device_address_->value();
    quint8 function_code = GetClientFunctionCode();

    qCInfo(kLoggingCategory) << "[SendReadRequest]"
                              << "register type:" << register_type
                              << "start address:" << start_address
                              << "quantity:" << quantity
                              << "server address:" << server_address;

    typedef QModbusDataUnit::RegisterType RegisterType;
    RegisterType type = static_cast<RegisterType>(register_type);
    QModbusDataUnit data_unit(type, start_address, quantity);
    QModbusClient *client = qobject_cast<QModbusClient*>(modbus_device_);
    QModbusReply *reply = client->sendReadRequest(data_unit, server_address);
    if (!SAKModbusFactory::Instance()->IsValidModbusReply(reply)) {
        return;
    }

    QString info = ui_->function_code_->currentText();
    OutputMessage(info, false, TXCOLOR, TXFLAG);
    connect(reply, &QModbusReply::finished, this, [=](){
        OutputModbusReply(reply, function_code);

        if (reply->error() == QModbusDevice::NoError) {
            UpdateClientTableViewData(reply->result().values());
            reply->deleteLater();
        }
    });
}

void SAKModbusUi::OnWriteClicked() {
    if (!IsConnected()) {
        return;
    }

    int registerType = ui_->function_code_->currentData().toInt();
    int start_address = ui_->start_address_->value();
    int server_address = ui_->device_address_->value();
    quint8 function_code = GetClientFunctionCode();
    QList<quint16> values = GetClientRegisterValue();
    SAKModbusFactory *factory = SAKModbusFactory::Instance();
    QModbusReply *reply = factory->SendWriteRequest(modbus_device_,
                                                    registerType,
                                                    start_address,
                                                    values,
                                                    server_address);
    if (SAKModbusFactory::Instance()->IsValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = ui_->function_code_->currentText();
        OutputMessage(info, false, TXCOLOR, TXFLAG);
    }
}

void SAKModbusUi::OnSendClicked() {
    if (!IsConnected()) {
        return;
    }

    quint8 server_address = ui_->device_address_->value();
    QByteArray pdu = GetClientPdu();
    QByteArray data = pdu;
    if (!data.isEmpty()) {
        data = data.remove(0, 1);
    }

    int function_code = int(QModbusDataUnit::Invalid);
    if (!pdu.isEmpty()) {
        function_code = pdu.at(0);
    }
    SAKModbusFactory *factory = SAKModbusFactory::Instance();
    QModbusReply *reply = factory->SendRawRequest(modbus_device_,
                                                  server_address,
                                                  function_code,
                                                  data);

    qCWarning(kLoggingCategory) << "Send raw request:"
                                << "server address:" << server_address
                                << "function code:" << function_code
                                << "data:" << QString(pdu.toHex(' '));
    if (SAKModbusFactory::Instance()->IsValidModbusReply(reply)) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, function_code);
            reply->deleteLater();
        });

        QString info = "pdu(No server address, no crc):";
        info += QString(pdu.toHex(' '));
        OutputMessage(info, false, TXCOLOR, TXFLAG);
    }

    // Update settings data.
    int index = settings_->value(key_ctx_->send_history_index).toInt();
    bool ret = WriteSettingsArray(key_ctx_->send_history,
                                  key_ctx_->pdu,
                                  QString(pdu.toHex(' ')),
                                  index,
                                  MAX_HISTORY_INDEX);
    if (!ret) {
        return;
    }

    if (index > ui_->pdu_->count()) {
        ui_->pdu_->addItem(QString(pdu.toHex(' ')));
    } else {
        ui_->pdu_->insertItem(index, QString(pdu.toHex(' ')));
    }

    index = index + 1 > MAX_HISTORY_INDEX ? 0 : index + 1;
    settings_->setValue(key_ctx_->send_history_index, index);
}

void SAKModbusUi::OnDateWritten(QModbusDataUnit::RegisterType table,
                                int address, int size) {
    qCInfo(kLoggingCategory) << "Data written:"
                             << "table:" << table
                             << "start address:" << address
                             << "size:" << size;
    QTableView *tv = GetTableView(table);
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
    QList<quint16> data = SAKModbusFactory::Instance()->GetServerData(server,
                                                                      table,
                                                                      address,
                                                                      size);
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


void SAKModbusUi::OnItemChanged(QStandardItem *item) {
    if (!item) {
        return;
    }

    if (item->column() != 1) {
        return;
    }

    if (SAKModbusFactory::Instance()->IsServerDevice(modbus_device_)) {
        int address = item->row();
        int current_index = ui_->server_registers_->currentIndex();
        QModbusDataUnit::RegisterType table = QModbusDataUnit::Invalid;
        if (current_index == 0) {
            table = QModbusDataUnit::Coils;
        } else if (current_index == 1) {
            table = QModbusDataUnit::DiscreteInputs;
        }  else if (current_index == 2) {
            table = QModbusDataUnit::InputRegisters;
        }  else if (current_index == 3) {
            table = QModbusDataUnit::HoldingRegisters;
        } else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow table type!");
            return;
        }

        quint16 value = item->text().toInt(Q_NULLPTR, 16);
        SAKModbusFactory::Instance()->SetServerData(modbus_device_,
                                                    table,
                                                    address,
                                                    value);
    }
}

QModbusDevice *SAKModbusUi::CreateModbusDevice() {
    QModbusDevice *device = Q_NULLPTR;
    int type = ui_->device_list_->currentData().toInt();
    if (SAKModbusFactory::Instance()->IsRtuSerialDeviceType(type)) {
        QString port_name = ui_->port_name_->currentText();
        int parity = ui_->parity_->currentData().toInt();
        int baud_rate = ui_->baud_rate_->currentData().toInt();
        int data_bits = ui_->data_bits_->currentData().toInt();
        int stop_bits = ui_->stop_bits_->currentData().toInt();
        SAKModbusFactory *factory = SAKModbusFactory::Instance();
        device = factory->CreateRtuSerialDevice(type,
                                                port_name,
                                                parity,
                                                baud_rate,
                                                data_bits,
                                                stop_bits);
    } else if (SAKModbusFactory::Instance()->IsTcpDeviceType(type)) {
        QString address = ui_->address_combo_box->currentText();
        int port = ui_->port_spin_box->value();
        SAKModbusFactory *factory = SAKModbusFactory::Instance();
        device = factory->CreateTcpDevice(type, address, port);
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown device type");
    }

    return device;
}

QTableView *SAKModbusUi::CreateTableView(int row_count, QTableView *table_view) {
    if (!table_view) {
        table_view = new QTableView(this);
    }

    QHeaderView *hv = table_view->horizontalHeader();
    QStandardItemModel *model = new QStandardItemModel(table_view);
    QStringList labels = QStringList() << tr("Address")
                                       << tr("Value")
                                       << tr("Description");
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

    connect(model, &QStandardItemModel::itemChanged,
            this, &SAKModbusUi::OnItemChanged);

    return table_view;
}

void SAKModbusUi::UpdateUiState(bool connected) {
    ui_->device_list_->setEnabled(!connected);
    ui_->cloese_button_->setEnabled(connected);
    ui_->open_button_->setEnabled(!connected);
    ui_->networkGroupBox->setEnabled(!connected);
    ui_->serialPortGroupBox->setEnabled(!connected);
#if 0
    ui->clientGroupBox->setEnabled(!connected);
    ui->serverGroupBox->setEnabled(!connected);
#endif
}

void SAKModbusUi::UpdateClientTableView() {
    int number = ui_->quantity_->value();
    int rowCount = register_model_->rowCount();
    if (number > rowCount) {
        register_model_->insertRows(rowCount, number - rowCount);
    } else if (number < rowCount) {
        register_model_->removeRows(number, rowCount - number);
    }

    // Update address.
    int start_address = ui_->start_address_->value();
    UpdateClientTableViewAddress(ui_->client_registers_, start_address);
}

void SAKModbusUi::UpdateClientTableViewData(const QList<quint16> &values) {
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
    ui_->client_registers_->viewport()->update();
}

void SAKModbusUi::UpdateClientReadWriteButtonState() {
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

void SAKModbusUi::UpdateClientParameters() {
    int timeout = ui_->timeout_->value();
    int repeat_time = ui_->repeat_time_->value();
    SAKModbusFactory::Instance()->SetClientDeviceParameters(modbus_device_,
                                                            timeout,
                                                            repeat_time);
}

void SAKModbusUi::UpdateClientTableViewAddress(QTableView *view,
                                               int start_address) {
    auto *model = qobject_cast<QStandardItemModel*>(view->model());
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

void SAKModbusUi::UpdateServerParameters() {
    bool is_busy = ui_->is_busy_->isChecked();
    bool listen_only = ui_->just_listen_->isChecked();
    int address = ui_->server_address->value();
    SAKModbusFactory::Instance()->SetServerDeviceParameters(modbus_device_,
                                                            address,
                                                            is_busy,
                                                            listen_only);
}

bool SAKModbusUi::UpdateServerMap(QModbusDevice *server) {
    if (server && qobject_cast<QModbusServer*>(server)) {
        QVector<quint16> values(UINT16_MAX + 1, 0);
        QModbusDataUnit dataUnit(QModbusDataUnit::Coils, 0, values);

        QModbusDataUnitMap map;
        map.insert(QModbusDataUnit::Coils, dataUnit);
        map.insert(QModbusDataUnit::DiscreteInputs, dataUnit);
        map.insert(QModbusDataUnit::HoldingRegisters, dataUnit);
        map.insert(QModbusDataUnit::InputRegisters, dataUnit);

        QModbusServer *cooked_server = qobject_cast<QModbusServer*>(server);
        cooked_server->blockSignals(true);
        bool is_ok = cooked_server->setMap(map);
        cooked_server->blockSignals(false);
        return is_ok;
    }

    return false;
}

void SAKModbusUi::UpdateServerRegistersData() {
    for (int i = 0; i < 4; i++) {
        QWidget *widget = ui_->server_registers_->widget(i);
        QTableView *table_view = qobject_cast<QTableView*>(widget);
        auto *model = qobject_cast<QStandardItemModel*>(table_view->model());
        int type = QModbusDataUnit::Invalid;
        if (i == 0) {
            type = QModbusDataUnit::Coils;
        } else if (i == 1) {
            type = QModbusDataUnit::DiscreteInputs;
        } else if (i == 2) {
            type = QModbusDataUnit::InputRegisters;
        }  else if (i == 3) {
            type = QModbusDataUnit::HoldingRegisters;
        } else {
            qCWarning(kLoggingCategory) << "Unknown register type.";
            continue;
        }

        for (int row = 0; row < model->rowCount(); row++) {
            QStandardItem *item = model->item(row, 1);
            quint16 value = item ? item->text().toInt(Q_NULLPTR, 16) : 0;
            auto table = static_cast<QModbusDataUnit::RegisterType>(type);
            SAKModbusFactory::Instance()->SetServerData(modbus_device_,
                                                        table,
                                                        row,
                                                        value,
                                                        false);
        }
    }
}

quint8 SAKModbusUi::GetClientFunctionCode() {
    QString txt = ui_->function_code_->currentText();
    QStringList list = txt.split('-', Qt::SkipEmptyParts);
    if (list.length()) {
        return list.first().toInt(Q_NULLPTR, 16);
    }

    return 0;
}

QList<quint16> SAKModbusUi::GetClientRegisterValue() {
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

QByteArray SAKModbusUi::GetClientPdu() {
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

QTableView *SAKModbusUi::GetTableView(QModbusDataUnit::RegisterType table) {
    QWidget *tv = Q_NULLPTR;
    if (table == QModbusDataUnit::Coils) {
        tv = ui_->server_registers_->widget(0);
    } else if (table == QModbusDataUnit::DiscreteInputs) {
        tv = ui_->server_registers_->widget(1);
    } else if (table == QModbusDataUnit::InputRegisters) {
        tv = ui_->server_registers_->widget(2);
    } else if (table == QModbusDataUnit::HoldingRegisters) {
        tv = ui_->server_registers_->widget(3);
    } else {
        tv = ui_->server_registers_->widget(3);
        qCWarning(kLoggingCategory)
                << "Get table view failed: unknow register type!";
    }

    return qobject_cast<QTableView*>(tv);
}

QList<quint16>
SAKModbusUi::GetTableValues(QTableView *table_view, int row, int count) {
    if (!table_view) {
        qCWarning(kLoggingCategory) << "Table view can not be null!";
        return QVector<quint16>();
    }

    auto *model = qobject_cast<QStandardItemModel*>(table_view->model());
    if (!model) {
        qCWarning(kLoggingCategory) << "Model can not be null!";
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

void SAKModbusUi::OutputModbusReply(QModbusReply *reply, int function_code) {
    if (reply->error() != QModbusDevice::NoError) {
        OutputMessage(reply->errorString(), true, "", "error");
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
                           .arg(QString::fromLatin1(data.toHex(' ')));
        OutputMessage(info, false, RXCOLOR, RXFLAG);
    } else if (reply->type() == QModbusReply::ReplyType::Common) {
        QString info = ui_->function_code_->currentText();
        OutputMessage(info, false, RXCOLOR, RXFLAG);
    }
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

bool SAKModbusUi::IsConnected() {
    if (SAKModbusFactory::Instance()->IsConnected(modbus_device_)) {
        return true;
    }

    QMessageBox::warning(this,
                         tr("Device is not Ready"),
                         tr("The modbus device is not ready, "
                            "please check settings and try again later!"));
    return false;
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
