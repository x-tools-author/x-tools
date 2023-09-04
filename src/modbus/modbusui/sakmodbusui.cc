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

#include "SAKSettings.hh"
#include "sakmodbusui.hh"
#include "ui_sakmodbusui.h"

#define RXCOLOR "green"
#define TXCOLOR "blue"
#define RXFLAG "rx:"
#define TXFLAG "tx:"
#define MAX_HISTORY_INDEX 9

class ReadOnlyDelegate: public QItemDelegate {
public:
    ReadOnlyDelegate(QWidget *parent = NULL):QItemDelegate(parent){}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        Q_UNUSED(parent);
        Q_UNUSED(option);
        Q_UNUSED(index);
        return NULL;
    }
};

SAKModbusUi::SAKModbusUi(QWidget *parent)
    : QWidget{parent}
    , ui_(new Ui::SAKModbusUi)
    , modbus_device_(Q_NULLPTR)
    , register_model(Q_NULLPTR)
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
    ui_->device_list->addItem(tr("RtuClient"), ModbusDeviceRtuSerialClient);
    ui_->device_list->addItem(tr("RtuServer"), ModbusDeviceRtuSerialServer);
    ui_->device_list->addItem(tr("TcpClient"), ModbusDeviceTcpClient);
    ui_->device_list->addItem(tr("TcpServer"), ModbusDeviceTcpServer);
}

void SAKModbusUi::InitComponentAddress() {
    ui_->addressComboBox->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui_->addressComboBox->addItem(address.toString());
        }
    }
}

void SAKModbusUi::InitComponentPortName() {
    ui_->portNameComboBox->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui_->portNameComboBox->addItem(info.portName());
    }
}

void SAKModbusUi::InitComponnetBaudRate() {
    ui_->baudRateComboBox->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui_->baudRateComboBox->addItem(QString::number(bd), bd);
    }
}

void SAKModbusUi::InitComponnetDataBits() {
    ui_->dataBitsComboBox->clear();
    ui_->dataBitsComboBox->addItem("8", QSerialPort::Data8);
    ui_->dataBitsComboBox->addItem("7", QSerialPort::Data7);
    ui_->dataBitsComboBox->addItem("6", QSerialPort::Data6);
    ui_->dataBitsComboBox->addItem("5", QSerialPort::Data5);
}

void SAKModbusUi::InitComponnetStopBits() {
    ui_->stopBitsComboBox->clear();
    ui_->stopBitsComboBox->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsComboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui_->stopBitsComboBox->addItem("2", QSerialPort::TwoStop);
}

void SAKModbusUi::InitComponnetParity() {
    ui_->parityComboBox->clear();
    ui_->parityComboBox->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui_->parityComboBox->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui_->parityComboBox->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui_->parityComboBox->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui_->parityComboBox->addItem(tr("MarkParity"), QSerialPort::MarkParity);
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

    ui_->functionCodeComboBox->addItem(str0x01, func0x01);
    ui_->functionCodeComboBox->addItem(str0x02, func0x02);
    ui_->functionCodeComboBox->addItem(str0x03, func0x03);
    ui_->functionCodeComboBox->addItem(str0x04, func0x04);
    ui_->functionCodeComboBox->addItem(str0x05, func0x05);
    ui_->functionCodeComboBox->addItem(str0x06, func0x06);
    ui_->functionCodeComboBox->addItem(str0x0f, func0x0f);
    ui_->functionCodeComboBox->addItem(str0x10, func0x10);
}

void SAKModbusUi::InitComponentRegisterTableView() {
    QTableView *tv = TableViewInit(1, ui_->registerTableView);
    register_model = reinterpret_cast<QStandardItemModel*>(tv->model());
}

void SAKModbusUi::InitComponentInput() {
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui_->pduComboBox->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void SAKModbusUi::InitComponentRegisterTabWidget() {
    QTabWidget *tw = ui_->registerTabWidget;
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
    if (deviceIndex >= 0 && deviceIndex < ui_->device_list->count()) {
        ui_->device_list->setCurrentIndex(deviceIndex);
    }
}

void SAKModbusUi::InitSettingsNetwork() {
    QString address = settings_->value(mSettingsKeyCtx.address).toString();
    ui_->addressComboBox->setCurrentText(address);

    QVariant portValiant = settings_->value(mSettingsKeyCtx.port);
    int port = portValiant.toInt();
    if (portValiant.isNull()) {
        port = 502;
    }
    ui_->portSpinBox->setValue(port);
}

void SAKModbusUi::InitSettingsSerialPort() {
    auto setComboBoxIndex = [](int index, QComboBox *cb){
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    };

    QString portName = settings_->value(mSettingsKeyCtx.portName).toString();
    int index = ui_->portNameComboBox->findText(portName);
    setComboBoxIndex(index, ui_->portNameComboBox);

    index = settings_->value(mSettingsKeyCtx.parity).toInt();
    setComboBoxIndex(index, ui_->parityComboBox);

    QString bd = settings_->value(mSettingsKeyCtx.baudRate).toString();
    ui_->baudRateComboBox->setCurrentText(bd);

    index = settings_->value(mSettingsKeyCtx.dataBits).toInt();
    setComboBoxIndex(index, ui_->dataBitsComboBox);

    index = settings_->value(mSettingsKeyCtx.stopBits).toInt();
    setComboBoxIndex(index, ui_->stopBitsComboBox);
}

void SAKModbusUi::InitSettingsClient() {
    int timeout = settings_->value(mSettingsKeyCtx.clientTimeout).toInt();
    ui_->clientTimeoutSpinBox->setValue(timeout < 100 ? 100 : timeout);

    int repeatTimes =
        settings_->value(mSettingsKeyCtx.clientRepeatTime).toInt();
    ui_->clientRepeatTimeSpinBox->setValue(repeatTimes);
}

void SAKModbusUi::InitSettingsServer() {
    bool isBusy = settings_->value(mSettingsKeyCtx.serverIsBusy).toBool();
    ui_->serverIsBusyCheckBox->setChecked(isBusy);

    bool justListen = settings_->value(mSettingsKeyCtx.serverJustListen).toBool();
    ui_->serverJustListenCheckBox->setChecked(justListen);

    int address = settings_->value(mSettingsKeyCtx.serverAddress).toInt();
    ui_->serverAddressSpinBox->setValue(address);
}

void SAKModbusUi::InitSettingsClientOperations() {
    int index = settings_->value(mSettingsKeyCtx.functionCode).toInt();
    if (index >= 0 && index < ui_->functionCodeComboBox->count()) {
        ui_->functionCodeComboBox->setCurrentIndex(index);
    }

    int address = settings_->value(mSettingsKeyCtx.targetAddress).toInt();
    ui_->targetAddressSpinBox->setValue(address);

    int start = settings_->value(mSettingsKeyCtx.startAddress).toInt();
    ui_->startAddressSpinBox->setValue(start);

    int number = settings_->value(mSettingsKeyCtx.addressNumber).toInt();
    ui_->addressNumberSpinBox->setValue(number);
}

void SAKModbusUi::InitSettingsInput() {
    ui_->pduComboBox->clear();
    settings_->beginReadArray(mSettingsKeyCtx.sendHistory);
    for (int i = 0; i < 10; i++) {
        settings_->setArrayIndex(i);
        QString text = settings_->value(mSettingsKeyCtx.pdu).toString();
        if (!text.isEmpty()) {
            ui_->pduComboBox->addItem(text);
        }
    }
    settings_->endArray();

    int index = settings_->value(mSettingsKeyCtx.sendHistoryIndex).toInt();
    ui_->pduComboBox->setCurrentIndex(index - 1);
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
    connect(ui_->device_list,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnDeviceTypeChanged);
    connect(ui_->openDevicePushButton, &QPushButton::clicked,
            this, &SAKModbusUi::OnInvokeOpen);
    connect(ui_->closeDevicePushButton, &QPushButton::clicked,
            this, &SAKModbusUi::OnInvokeClose);

}

void SAKModbusUi::InitSignalsNetworking() {
    connect(ui_->addressComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnAddressChanged);
    connect(ui_->portSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnPortChanged);
}

void SAKModbusUi::InitSignalsSerialPort() {
    connect(ui_->portNameComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnPortNameChanged);
    connect(ui_->parityComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnParityChanged);
    connect(ui_->baudRateComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnBaudRateChanged);
    connect(ui_->dataBitsComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnDataBitsChanged);
    connect(ui_->stopBitsComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusUi::OnStopBistChanged);
}

void SAKModbusUi::InitSignalsClient() {
    connect(ui_->clientTimeoutSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnClientTimeoutChanged);
    connect(ui_->clientRepeatTimeSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnClientRepeatTimeChanged);
}

void SAKModbusUi::InitSignalsServer() {
    connect(ui_->serverIsBusyCheckBox, &QCheckBox::clicked,
            this, &SAKModbusUi::OnServerIsBusyChanged);
    connect(ui_->serverJustListenCheckBox, &QCheckBox::clicked,
            this, &SAKModbusUi::OnServerJustListenChanged);
    connect(ui_->serverAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnServerAddressChanged);
}

void SAKModbusUi::InitSignalsClientOperations() {
    connect(ui_->functionCodeComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusUi::OnFunctionCodeChanged);
    connect(ui_->targetAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnTargetAddressChanged);
    connect(ui_->startAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnStartAddressChanged);
    connect(ui_->addressNumberSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusUi::OnAddressNumberChanged);
    connect(ui_->readPushButton, &QPushButton::clicked,
            this, &SAKModbusUi::ClientRead);
    connect(ui_->writePushButton, &QPushButton::clicked,
            this, &SAKModbusUi::ClientWrite);
    connect(ui_->sendPushButton, &QPushButton::clicked,
            this, &SAKModbusUi::ClientSend);
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
    int type = ui_->device_list->currentData().toInt();
    bool isSerial =
        (type == ModbusDeviceRtuSerialClient
         || type == ModbusDeviceRtuSerialServer);
    bool isClient =
        (type == ModbusDeviceRtuSerialClient
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

void SAKModbusUi::OnInvokeOpen() {
    ui_->openDevicePushButton->setEnabled(false);
    if (modbus_device_) {
        modbus_device_->disconnectDevice();
        modbus_device_->deleteLater();
        modbus_device_ = Q_NULLPTR;
    }

    int type = ui_->device_list->currentData().toInt();
    if (type == ModbusDeviceRtuSerialClient
        || type == ModbusDeviceRtuSerialServer) {
        QString portName = ui_->portNameComboBox->currentText();
        int parity = ui_->parityComboBox->currentData().toInt();
        int baudRate = ui_->baudRateComboBox->currentData().toInt();
        int dataBits = ui_->dataBitsComboBox->currentData().toInt();
        int stopBits = ui_->stopBitsComboBox->currentData().toInt();
        modbus_device_ = CreateRtuSerialDevice(
                    type, portName, parity, baudRate, dataBits, stopBits);
    } else {
        QString address = ui_->addressComboBox->currentText();
        int port = ui_->portSpinBox->value();
        modbus_device_ = CreateTcpDevice(type, address, port);
    }

    if (!modbus_device_) {
        ui_->openDevicePushButton->setEnabled(true);
        qCWarning(logging_category_) << "Can not create modbus devices!";
        return;
    }

    if (IsServer(modbus_device_)) {
        if (!ResetServerMap(modbus_device_)) {
            qCWarning(logging_category_) << "Can not reset server map!";
            return;
        }

        bool isBusy = ui_->serverIsBusyCheckBox->isChecked();
        bool listenOnly = ui_->serverJustListenCheckBox->isChecked();
        int address = ui_->serverAddressSpinBox->value();
        SetServerParameters(
            modbus_device_, QModbusServer::DeviceBusy, isBusy);
        SetServerParameters(
            modbus_device_, QModbusServer::ListenOnlyMode, listenOnly);

        QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
        server->setServerAddress(address);
        SynchronizationRegister(server);
        connect(server, &QModbusServer::dataWritten, this,
                [=](QModbusDataUnit::RegisterType table, int address, int size){
            TableViewUpdateData(table, address, size);
        });
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
                       tr("Can not open device: ")
                           + errStr
                           + tr(". Please check the parameters and try again!"));
    }
}

void SAKModbusUi::OnAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.address,
                        ui_->addressComboBox->currentText());
}

void SAKModbusUi::OnPortChanged() {
    settings_->setValue(mSettingsKeyCtx.port,
                        ui_->portSpinBox->value());
}

void SAKModbusUi::OnPortNameChanged() {
    settings_->setValue(mSettingsKeyCtx.portName,
                        ui_->portNameComboBox->currentText());
}

void SAKModbusUi::OnParityChanged() {
    settings_->setValue(mSettingsKeyCtx.parity,
                        ui_->parityComboBox->currentIndex());
}

void SAKModbusUi::OnBaudRateChanged() {
    settings_->setValue(mSettingsKeyCtx.baudRate,
                        ui_->baudRateComboBox->currentText());
}

void SAKModbusUi::OnDataBitsChanged() {
    settings_->setValue(mSettingsKeyCtx.dataBits,
                        ui_->dataBitsComboBox->currentIndex());
}

void SAKModbusUi::OnStopBistChanged() {
    settings_->setValue(mSettingsKeyCtx.stopBits,
                        ui_->stopBitsComboBox->currentIndex());
}

void SAKModbusUi::OnInvokeRefresh() {
    InitComponentPortName();
}

void SAKModbusUi::OnClientTimeoutChanged() {
    settings_->setValue(mSettingsKeyCtx.clientTimeout,
                        ui_->clientTimeoutSpinBox->value());
    ClientUpdateParameters();
}

void SAKModbusUi::OnClientRepeatTimeChanged() {
    settings_->setValue(mSettingsKeyCtx.clientRepeatTime,
                        ui_->clientRepeatTimeSpinBox->value());
    ClientUpdateParameters();
}

void SAKModbusUi::OnServerIsBusyChanged() {
    settings_->setValue(mSettingsKeyCtx.serverIsBusy,
                        ui_->serverIsBusyCheckBox->isChecked());
    if (IsServer(modbus_device_)) {
        SetServerParameters(modbus_device_, QModbusServer::DeviceBusy,
                                    ui_->serverIsBusyCheckBox->isChecked());
    }
}

void SAKModbusUi::OnServerJustListenChanged() {
    settings_->setValue(mSettingsKeyCtx.serverJustListen,
                        ui_->serverJustListenCheckBox->isChecked());
    SetServerParameters(modbus_device_, QModbusServer::ListenOnlyMode,
                        ui_->serverJustListenCheckBox->isChecked());
}

void SAKModbusUi::OnServerAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.serverAddress,
                        ui_->serverAddressSpinBox->value());
    if (IsServer(modbus_device_)) {
        QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);
        server->setServerAddress(ui_->serverAddressSpinBox->value());
    }
}

void SAKModbusUi::OnFunctionCodeChanged() {
    settings_->setValue(mSettingsKeyCtx.functionCode,
                        ui_->functionCodeComboBox->currentIndex());
    ClientUpdateRWBtState();
}

void SAKModbusUi::OnTargetAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.targetAddress,
                        ui_->targetAddressSpinBox->value());
}

void SAKModbusUi::OnStartAddressChanged() {
    settings_->setValue(mSettingsKeyCtx.startAddress,
                        ui_->startAddressSpinBox->value());
    ClientUpdateTable();
}

void SAKModbusUi::OnAddressNumberChanged() {
    settings_->setValue(mSettingsKeyCtx.addressNumber,
                        ui_->addressNumberSpinBox->value());
    ClientUpdateTable();
}

void SAKModbusUi::UpdateUiState(bool opened) {
    ui_->device_list->setEnabled(!opened);
    ui_->closeDevicePushButton->setEnabled(opened);
    ui_->openDevicePushButton->setEnabled(!opened);
    ui_->networkGroupBox->setEnabled(!opened);
    ui_->serialPortGroupBox->setEnabled(!opened);
#if 0
    ui->clientGroupBox->setEnabled(!opened);
    ui->serverGroupBox->setEnabled(!opened);
#endif
}

quint16 SAKModbusUi::CalculateModbusCrc(const QByteArray &data) {
    int len = data.size();
    uint16_t wcrc = 0XFFFF;
    uint8_t temp;
    for (int i = 0; i < len; i++) {
       temp = data.at(i);
       wcrc ^= temp;
       for (int j = 0; j < 8; j++){
          if ( wcrc & 0X0001){
              wcrc >>= 1;
              wcrc ^= 0XA001;
          } else {
              wcrc>>=1;
          }
       }
    }
    return wcrc;
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
        QWidget *widget = ui_->registerTabWidget->widget(i);
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
                SetServerData(server, table, row, value);
            }
        }
    }
}

bool SAKModbusUi::WriteSettingsArray(const QString &group, const QString &key,
                                     const QString &value, int index,
                                     int maxIndex) {
    settings_->beginWriteArray(group);
    for (int i = 0; i < maxIndex; i++) {
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

void SAKModbusUi::ClientRead()
{
    if (!modbus_device_) {
        ShowMessageBoxDeviceIsNotReady();
        return;
    }

    int registerType = ui_->functionCodeComboBox->currentData().toInt();
    quint16 startAddress = ui_->startAddressSpinBox->value();
    quint16 addressNumber = ui_->addressNumberSpinBox->value();
    quint16 targetAddress = ui_->targetAddressSpinBox->value();
    quint8 functionCode = this->ClientFunctionCode();
    QModbusReply *reply = SendReadRequest(modbus_device_, registerType,
                                          startAddress, addressNumber,
                                          targetAddress);

    if (IsValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, functionCode);

            if (reply->error() == QModbusDevice::NoError) {
                QJsonArray result = ModbusReplyResult(reply);
                ClientSetRegisterValue(result);
                reply->deleteLater();
            }
        });

        OutputModbusRequestRead(targetAddress, functionCode,
                                startAddress, addressNumber);
    }
}

void SAKModbusUi::ClientWrite() {
    if (!modbus_device_) {
        ShowMessageBoxDeviceIsNotReady();
        return;
    }

    int registerType = ui_->functionCodeComboBox->currentData().toInt();
    int startAddress = ui_->startAddressSpinBox->value();
    int targetAddress = ui_->targetAddressSpinBox->value();
    int addressNumber = ui_->addressNumberSpinBox->value();
    quint8 functionCode = this->ClientFunctionCode();
    QJsonArray values = ClientRegisterValue();
    QModbusReply *reply = SendWriteRequest(
        modbus_device_, registerType, startAddress, values, targetAddress);
    if (IsValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, functionCode);
            reply->deleteLater();
        });

        OutputModbusRequestWrite(targetAddress, functionCode, startAddress,
                                 addressNumber, values);
    }
}

void SAKModbusUi::ClientSend() {
    if (!modbus_device_) {
        ShowMessageBoxDeviceIsNotReady();
        return;
    }

    quint8 serverAddress = ui_->targetAddressSpinBox->value();
    QByteArray pdu = ClientPdu();
    QByteArray data = pdu.length()
            ? QByteArray(pdu.data() + 1, pdu.length() - 1)
            : QByteArray();
    int functionCode = pdu.count() ? pdu.at(0) : int(QModbusDataUnit::Invalid);
    QModbusReply *reply = SendRawRequest(
        modbus_device_, serverAddress, functionCode, data);

    qCWarning(logging_category_) << "address:" << serverAddress
                                << "function code:" << functionCode
                                << "data:" << QString(pdu.toHex(' '));

    if (IsValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            OutputModbusReply(reply, functionCode);
            reply->deleteLater();
        });

        OutputModbusRequestSend(serverAddress, pdu);
    }

    // Update settings data.
    int index = settings_->value(mSettingsKeyCtx.sendHistoryIndex).toInt();
    bool ret = WriteSettingsArray(
        mSettingsKeyCtx.sendHistory, mSettingsKeyCtx.pdu,
        QString(pdu.toHex(' ')), index, MAX_HISTORY_INDEX);
    if (ret) {
        if (index > ui_->pduComboBox->count()) {
            ui_->pduComboBox->addItem(QString(pdu.toHex(' ')));
        } else {
            ui_->pduComboBox->insertItem(index, QString(pdu.toHex(' ')));
        }
        settings_->setValue(mSettingsKeyCtx.sendHistoryIndex,
                            index + 1 > MAX_HISTORY_INDEX ? 0 : index + 1);
    }
}

void SAKModbusUi::ClientUpdateTable() {
    int number = ui_->addressNumberSpinBox->value();
    int rowCount = register_model->rowCount();
    if (number > rowCount) {
        register_model->insertRows(rowCount, number - rowCount);
    } else if (number < rowCount) {
        register_model->removeRows(number, rowCount - number);
    }

    // Update address.
    int startAddress = ui_->startAddressSpinBox->value();
    TableViewUpdateAddress(ui_->registerTableView, startAddress);
}

void SAKModbusUi::ClientSetRegisterValue(QJsonArray values) {
    int columnCount = register_model->columnCount();
    for (int row = 0; row < values.count(); row++) {
        for (int column = 1; column < columnCount; column++) {
            int value = values.at(row).toInt();
            int base = (column < 4)
                           ? (column == 1 ? 2 : (column == 2 ? 10 : 16))
                           : (column == 4 ? 2 : (column == 5 ? 10 : 16));
            quint16 valueBE = qToBigEndian<quint16>(value);
            quint16 cookedValue =  column < 4 ? value : valueBE;

            QModelIndex index = register_model->index(row, column);
            QMap<int, QVariant> roles;
            int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
            QString cookedStr = QString::number(cookedValue, base)
                                    .rightJustified(width, '0', true);
            roles.insert(Qt::DisplayRole, cookedStr);
            register_model->setItemData(index, roles);
        }
    }

    // Refresh the view, or the new value will not be show.
    ui_->registerTableView->viewport()->update();
}

void SAKModbusUi::ClientUpdateRWBtState() {
    QStringList list = ui_->functionCodeComboBox->currentText().split('-');
    int code = list.length() ? list.first().toInt(Q_NULLPTR, 16) : 0;
    bool isReadingOperation = false;
    if (code == 0x01 || code == 0x02 || code == 0x03 || code == 0x04) {
        isReadingOperation = true;
    }

    if (code == 0x05 || code == 0x06) {
        ui_->addressNumberSpinBox->setValue(1);
    }

    ui_->readPushButton->setEnabled(isReadingOperation);
    ui_->writePushButton->setEnabled(!isReadingOperation);
}

void SAKModbusUi::ClientUpdateParameters() {
    if (IsClient(modbus_device_)) {
        int timeout = ui_->clientTimeoutSpinBox->value();
        int repeatTime = ui_->clientRepeatTimeSpinBox->value();
        SetClientParameters(modbus_device_, timeout, repeatTime);
    }
}

quint8 SAKModbusUi::ClientFunctionCode() {
    QStringList list = ui_->functionCodeComboBox->currentText().split('-');
    if (list.length()) {
        return list.first().toInt();
    }

    return 0;
}

QJsonArray SAKModbusUi::ClientRegisterValue() {
    QJsonArray array;
    for (int row = 0; row < register_model->rowCount(); row++) {
        // Get data from hex column.
        QStandardItem *item = register_model->item(row, 3);
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
    QString text = ui_->pduComboBox->currentText();
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

void SAKModbusUi::OutputModbusReply(QModbusReply *reply, int functionCode) {
    if (reply->error() != QModbusDevice::NoError) {
        OutputMessage(reply->errorString(), true, "", "error");
        this->ShowMessageBox(tr("Error occured"),
                             reply->errorString());
        return;
    }

    int serverAddress = reply->serverAddress();
    QByteArray data = reply->rawResult().data();
    data.prepend(char(functionCode));
    data.prepend(char(serverAddress));
    quint16 crc = CalculateModbusCrc(data);
    data.append(reinterpret_cast<char*>(&crc), 2);
    QString msg = QString::fromLatin1(data.toHex(' '));
    OutputMessage(msg, false, RXCOLOR,
                  RXFLAG + QString::number(serverAddress));
}

void SAKModbusUi::OutputModbusRequestSend(int serverAddress,
                                        const QByteArray &pdu) {
    QByteArray tx;
    tx.append(char(serverAddress));
    tx.append(pdu);
    quint16 crc = CalculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    OutputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(serverAddress));
}

void SAKModbusUi::OutputModbusRequestRead(int serverAddress, int functionCode,
                                        int startAddress, int addressNumber) {
    QByteArray tx;
    quint16 startAddressBE = qToBigEndian<quint16>(startAddress);
    quint16 addressNumberBE = qToBigEndian<quint16>(addressNumber);
    tx.append(char(serverAddress));
    tx.append(char(functionCode));
    tx.append(reinterpret_cast<char*>(&startAddressBE), 2);
    tx.append(reinterpret_cast<char*>(&addressNumberBE), 2);
    quint16 crc = CalculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    OutputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(serverAddress));
}

void SAKModbusUi::OutputModbusRequestWrite(int serverAddress, int functionCode,
                                         int startAddress, int addressNumber,
                                         QJsonArray values) {
    QByteArray bytes;
    for (int i = 0; i < values.count(); i++) {
        quint16 v = values.at(i).toInt();
        quint16 vBE = qToBigEndian<quint16>(v);
        bytes.append(reinterpret_cast<char*>(&vBE), 2);
    }

    QByteArray tx;
    quint16 startAddressBE = qToBigEndian<quint16>(startAddress);
    quint16 addressNumberBE = qToBigEndian<quint16>(addressNumber);
    tx.append(char(serverAddress));
    tx.append(char(functionCode));
    tx.append(reinterpret_cast<char*>(&startAddressBE), 2);
    tx.append(reinterpret_cast<char*>(&addressNumberBE), 2);
    tx.append(bytes);
    quint16 crc = CalculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    OutputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(serverAddress));
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
    ui_->textBrowser->append(cookedMsg);
}

QTableView *SAKModbusUi::TableView(QModbusDataUnit::RegisterType table) {
    QWidget *tv = Q_NULLPTR;
    if (table == QModbusDataUnit::DiscreteInputs) {
        tv = ui_->registerTabWidget->widget(0);
    } else if (table == QModbusDataUnit::Coils) {
        tv = ui_->registerTabWidget->widget(1);
    } else if (table == QModbusDataUnit::InputRegisters) {
        tv = ui_->registerTabWidget->widget(2);
    } else if (table == QModbusDataUnit::HoldingRegisters) {
        tv = ui_->registerTabWidget->widget(3);
    } else {
        tv = ui_->registerTabWidget->widget(3);
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
        TableViewUpdateRow(item);
    }
#endif

    connect(model, &QStandardItemModel::itemChanged,
            this, &SAKModbusUi::TableViewUpdateRow);

    return tv;
}

QVector<quint16> SAKModbusUi::TableValues(QTableView *tv, int row, int count) {
    if (!tv) {
        qCWarning(logging_category_) << __FUNCTION__
                                    << ": tv can not be null!";
        return QVector<quint16>();
    }

    auto *model = qobject_cast<QStandardItemModel*>(tv->model());
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

void SAKModbusUi::TableViewUpdateAddress(QTableView *tv,
                                               int startAddress) {
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

void SAKModbusUi::TableViewUpdateRow(QStandardItem *item) {
    if (!item) {
        return;
    }

    if (item->column() == 0) {
        return;
    }

    QStandardItemModel *model = item->model();
    model->blockSignals(true);
    int row = item->row();
    int column = item->column();
    QString text = item->text();

    int columnCount = model->columnCount();
    int base = (column == 1) ? 2 : (column == 2 ? 10 : 16);
    quint16 value = CookedRegisterString(text, base);
    for (int column = 1; column < columnCount; column++) {
        int base = column < 4
                       ? (column == 1 ? 2 : (column == 2 ? 10 : 16))
                       : (column == 4 ? 2 : (column == 5 ? 10 : 16));
        quint16 valueBE = qToBigEndian<quint16>(value);
        quint16 cookedValue =  column < 4 ? value : valueBE;

        QModelIndex index = model->index(row, column);
        QMap<int, QVariant> roles;
        int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
        QString cookedStr = QString::number(cookedValue, base)
                                .rightJustified(width, '0', true);
        roles.insert(Qt::DisplayRole, cookedStr);
        model->setItemData(index, roles);

        QStandardItem *itemTemp = model->item(row, column);
        if (itemTemp) {
            itemTemp->setTextAlignment(Qt::AlignCenter);
        }
    }

    model->blockSignals(false);

    if (IsServer(modbus_device_)) {
        int address = item->row();
        int currentIndex = ui_->registerTabWidget->currentIndex();
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

        SetServerData(modbus_device_, table, address, value);
    }
}

void SAKModbusUi::TableViewUpdateData(int table, int address, int size) {
    qCInfo(logging_category_) << "Data written, table:" << table
                             << ", start address:" << address
                             << ", size:" << size << ".";
    QTableView *tv = TableView(RegisterType(table));
    QStandardItemModel *model =
            qobject_cast<QStandardItemModel*>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer*>(modbus_device_);


    QJsonArray data = ServerValues(server, table, address, size);
    size = qMin<int>(data.count(), size);
    for (int i = 0; i < size; i++) {
        int row = address + i;
        int base = 10;
        int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
        int value = data.at(i).toInt();
        QString cookedStr = QString::number(value, base)
                                .rightJustified(width, '0', true);

        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, cookedStr);
        QModelIndex index = model->index(row, 2);
        model->setItemData(index, roles);

        QStandardItem *itemTemp = model->item(row, 2);
        if (itemTemp) {
            itemTemp->setTextAlignment(Qt::AlignCenter);
        }
    }

    tv->viewport()->update();
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
    if (device) {
        if (device->inherits("QModbusClient")) {
            return true;
        }
    }

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

bool SAKModbusUi::SetServerData(
        QModbusDevice *server, QModbusDataUnit::RegisterType table,
        quint16 address, quint16 data) {
    if (server) {
        if (server->inherits("QModbusServer")) {
            QModbusServer *modbusServer = qobject_cast<QModbusServer*>(server);
            return modbusServer->setData(table, address, data);
        }
    }

    return false;
}

QModbusReply *SAKModbusUi::SendReadRequest(
        QModbusDevice *device, int registerType, int startAddress,
        int size, int serverAddress) {
    if (device && IsClient(device)) {
        qCInfo(logging_category_) << "register-type:" << registerType
                                 << " start-address:" << startAddress
                                 << " register-number:" << size
                                 << " serverAddress:" << serverAddress;
        QModbusClient *client = qobject_cast<QModbusClient*>(device);
        QModbusDataUnit::RegisterType cookedRegisterType
            = static_cast<QModbusDataUnit::RegisterType>(registerType);
        QModbusDataUnit dataUnit(cookedRegisterType, startAddress, size);
        QModbusReply *reply = client->sendReadRequest(dataUnit, serverAddress);
        return reply;
    }

    return Q_NULLPTR;
}

QJsonArray SAKModbusUi::ServerValues(
        QModbusServer *server, int registerType, int address, int size) {
    QJsonArray values;
    if (server) {
        QModbusDataUnit::RegisterType table =
                static_cast<QModbusDataUnit::RegisterType>(registerType);
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

QModbusReply *SAKModbusUi::SendWriteRequest(
        QModbusDevice *device, int registerType, int startAddress,
        QJsonArray values, int serverAddress) {
    if (device && IsClient(device)) {
        QModbusDataUnit::RegisterType cookedRegisterType
            = static_cast<QModbusDataUnit::RegisterType>(registerType);
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
    if (reply.canConvert<QModbusReply*>()) {
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
