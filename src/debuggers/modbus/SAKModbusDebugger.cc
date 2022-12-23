/******************************************************************************
 * Copyright 2022 Qter(qsaker@outlook.com). All rights reserved.
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

#include "SAKModbusDebugger.hh"
#include "ui_SAKModbusDebugger.h"

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

SAKModbusDebugger::SAKModbusDebugger(QSettings *settings, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKModbusDebugger)
    , mModbusDevice(Q_NULLPTR)
    , mLoggingCategory("SAK.Modbus")
    , mSettings(settings)
    , mRegisterModel(Q_NULLPTR)
{
    if (!mSettings) {
        mSettings = new QSettings("settings.ini", QSettings::IniFormat, this);
    }

    ui->setupUi(this);
    initComponents();
    initSettings();
    initSignals();

    updateUiState(false);
    onDeviceTypeChanged();
    clientUpdateTable();
    clientUpdateRWBtState();
}

SAKModbusDebugger::SAKModbusDebugger(QSettings *settings,
                     const QString settingsGroup,
                     QSqlDatabase *sqlDatabase,
                     QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SAKModbusDebugger)
    , mModbusDevice(Q_NULLPTR)
    , mLoggingCategory("SAK.Modbus")
    , mSettings(settings)
    , mRegisterModel(Q_NULLPTR)
{
    Q_UNUSED(sqlDatabase);
    if (!mSettings) {
        mSettings = new QSettings("settings.ini", QSettings::IniFormat, this);
    }

    ui->setupUi(this);
    initComponents();
    initSettings();
    initSignals();

    updateUiState(false);
    onDeviceTypeChanged();
    clientUpdateTable();
    clientUpdateRWBtState();
}

SAKModbusDebugger::~SAKModbusDebugger()
{
    delete ui;
}

void SAKModbusDebugger::initComponents()
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

void SAKModbusDebugger::initComponentDevices()
{
    ui->devicesComboBox->addItem(tr("RtuClient"), ModbusDeviceRtuSerialClient);
    ui->devicesComboBox->addItem(tr("RtuServer"), ModbusDeviceRtuSerialServer);
    ui->devicesComboBox->addItem(tr("TcpClient"), ModbusDeviceTcpClient);
    ui->devicesComboBox->addItem(tr("TcpServer"), ModbusDeviceTcpServer);
}

void SAKModbusDebugger::initComponentAddress()
{
    ui->addressComboBox->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->addressComboBox->addItem(address.toString());
        }
    }
}

void SAKModbusDebugger::initComponentPortName()
{
    ui->portNameComboBox->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui->portNameComboBox->addItem(info.portName());
    }
}

void SAKModbusDebugger::initComponnetBaudRate()
{
    ui->baudRateComboBox->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui->baudRateComboBox->addItem(QString::number(bd), bd);
    }
}

void SAKModbusDebugger::initComponnetDataBits()
{
    ui->dataBitsComboBox->clear();
    ui->dataBitsComboBox->addItem("8", QSerialPort::Data8);
    ui->dataBitsComboBox->addItem("7", QSerialPort::Data7);
    ui->dataBitsComboBox->addItem("6", QSerialPort::Data6);
    ui->dataBitsComboBox->addItem("5", QSerialPort::Data5);
}

void SAKModbusDebugger::initComponnetStopBits()
{
    ui->stopBitsComboBox->clear();
    ui->stopBitsComboBox->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsComboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsComboBox->addItem("2", QSerialPort::TwoStop);
}

void SAKModbusDebugger::initComponnetParity()
{
    ui->parityComboBox->clear();
    ui->parityComboBox->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui->parityComboBox->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui->parityComboBox->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui->parityComboBox->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui->parityComboBox->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void SAKModbusDebugger::initComponentFunctionCode()
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

    ui->functionCodeComboBox->addItem(str0x01, func0x01);
    ui->functionCodeComboBox->addItem(str0x02, func0x02);
    ui->functionCodeComboBox->addItem(str0x03, func0x03);
    ui->functionCodeComboBox->addItem(str0x04, func0x04);
    ui->functionCodeComboBox->addItem(str0x05, func0x05);
    ui->functionCodeComboBox->addItem(str0x06, func0x06);
    ui->functionCodeComboBox->addItem(str0x0f, func0x0f);
    ui->functionCodeComboBox->addItem(str0x10, func0x10);
}

void SAKModbusDebugger::initComponentRegisterTableView()
{
    QTableView *tv = tableViewInit(1, ui->registerTableView);
    mRegisterModel = reinterpret_cast<QStandardItemModel*>(tv->model());
}

void SAKModbusDebugger::initComponentInput()
{
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui->pduComboBox->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void SAKModbusDebugger::initComponentRegisterTabWidget()
{
    QTabWidget *tw = ui->registerTabWidget;
    QStringList titles;
    titles << tr("DiscreteInputs") << tr("Coils")
           << tr("InputRegisters") << tr("HoldingRegisters");
    for (QString &title : titles) {
        QTableView *tv = tableViewInit(UINT16_MAX + 1, Q_NULLPTR);
        tv->verticalHeader()->hide();
        tw->addTab(tv, title);
    }
}

void SAKModbusDebugger::initSettings()
{
    initSettingsDevice();
    initSettingsNetwork();
    initSettingsSerialPort();
    initSettingsClient();
    initSettingsServer();
    initSettingsClientOperations();
    initSettingsInput();
}

void SAKModbusDebugger::initSettingsDevice()
{
    int deviceIndex = mSettings->value(mSettingsKeyCtx.deviceIndex).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui->devicesComboBox->count()) {
        ui->devicesComboBox->setCurrentIndex(deviceIndex);
    }
}

void SAKModbusDebugger::initSettingsNetwork()
{
    bool custom = mSettings->value(mSettingsKeyCtx.customAddress).toBool();
    ui->customAddressCheckBox->setChecked(custom);
    ui->addressComboBox->setEditable(custom);

    QString address = mSettings->value(mSettingsKeyCtx.address).toString();
    int index = ui->addressComboBox->findText(address);
    if (custom) {
        ui->addressComboBox->lineEdit()->setText(address);

    } else {
        if (index >= 0 && index < ui->addressComboBox->count()) {
            ui->addressComboBox->setCurrentIndex(index);
        }
    }

    QVariant portValiant = mSettings->value(mSettingsKeyCtx.port);
    int port = portValiant.toInt();
    if (portValiant.isNull()) {
        port = 502;
    }
    ui->portSpinBox->setValue(port);
}

void SAKModbusDebugger::initSettingsSerialPort()
{
    auto setComboBoxIndex = [](int index, QComboBox *cb){
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    };

    QString portName = mSettings->value(mSettingsKeyCtx.portName).toString();
    int index = ui->portNameComboBox->findText(portName);
    setComboBoxIndex(index, ui->portNameComboBox);

    index = mSettings->value(mSettingsKeyCtx.parity).toInt();
    setComboBoxIndex(index, ui->parityComboBox);

    bool customBR = mSettings->value(mSettingsKeyCtx.customBaudRate).toBool();
    ui->baudRateComboBox->setEditable(customBR);
    ui->customBaudRateCheckBox->setChecked(customBR);

    QString bd = mSettings->value(mSettingsKeyCtx.baudRate).toString();
    index = ui->baudRateComboBox->findText(bd);
    if (customBR) {
        ui->baudRateComboBox->lineEdit()->setText(bd);
    } else {
        setComboBoxIndex(index, ui->baudRateComboBox);
    }

    index = mSettings->value(mSettingsKeyCtx.dataBits).toInt();
    setComboBoxIndex(index, ui->dataBitsComboBox);

    index = mSettings->value(mSettingsKeyCtx.stopBits).toInt();
    setComboBoxIndex(index, ui->stopBitsComboBox);
}

void SAKModbusDebugger::initSettingsClient()
{
    int timeout = mSettings->value(mSettingsKeyCtx.clientTimeout).toInt();
    ui->clientTimeoutSpinBox->setValue(timeout < 100 ? 100 : timeout);

    int repeatTimes =
        mSettings->value(mSettingsKeyCtx.clientRepeatTime).toInt();
    ui->clientRepeatTimeSpinBox->setValue(repeatTimes);
}

void SAKModbusDebugger::initSettingsServer()
{
    bool isBusy = mSettings->value(mSettingsKeyCtx.serverIsBusy).toBool();
    ui->serverIsBusyCheckBox->setChecked(isBusy);

    bool justListen = mSettings->value(mSettingsKeyCtx.serverJustListen).toBool();
    ui->serverJustListenCheckBox->setChecked(justListen);

    int address = mSettings->value(mSettingsKeyCtx.serverAddress).toInt();
    ui->serverAddressSpinBox->setValue(address);
}

void SAKModbusDebugger::initSettingsClientOperations()
{
    int index = mSettings->value(mSettingsKeyCtx.functionCode).toInt();
    if (index >= 0 && index < ui->functionCodeComboBox->count()) {
        ui->functionCodeComboBox->setCurrentIndex(index);
    }

    int address = mSettings->value(mSettingsKeyCtx.targetAddress).toInt();
    ui->targetAddressSpinBox->setValue(address);

    int start = mSettings->value(mSettingsKeyCtx.startAddress).toInt();
    ui->startAddressSpinBox->setValue(start);

    int number = mSettings->value(mSettingsKeyCtx.addressNumber).toInt();
    ui->addressNumberSpinBox->setValue(number);
}

void SAKModbusDebugger::initSettingsInput()
{
    ui->pduComboBox->clear();
    mSettings->beginReadArray(mSettingsKeyCtx.sendHistory);
    for (int i = 0; i < 10; i++) {
        mSettings->setArrayIndex(i);
        QString text = mSettings->value(mSettingsKeyCtx.pdu).toString();
        if (!text.isEmpty()) {
            ui->pduComboBox->addItem(text);
        }
    }
    mSettings->endArray();

    int index = mSettings->value(mSettingsKeyCtx.sendHistoryIndex).toInt();
    ui->pduComboBox->setCurrentIndex(index - 1);
}

void SAKModbusDebugger::initSignals()
{
    initSignalsDevice();
    initSignalsNetworking();
    initSignalsSerialPort();
    initSignalsClient();
    initSignalsServer();
    initSignalsClientOperations();
}

void SAKModbusDebugger::initSignalsDevice()
{
    connect(ui->devicesComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusDebugger::onDeviceTypeChanged);
    connect(ui->openDevicePushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::onInvokeOpen);
    connect(ui->closeDevicePushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::onInvokeClose);

}

void SAKModbusDebugger::initSignalsNetworking()
{
    connect(ui->addressComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusDebugger::onAddressChanged);
    connect(ui->portSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onPortChanged);
    connect(ui->customAddressCheckBox, &QCheckBox::clicked,
            this, &SAKModbusDebugger::onCustomAddressChanged);

}

void SAKModbusDebugger::initSignalsSerialPort()
{
    connect(ui->portNameComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusDebugger::onPortNameChanged);
    connect(ui->parityComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusDebugger::onParityChanged);
    connect(ui->baudRateComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusDebugger::onBaudRateChanged);
    connect(ui->dataBitsComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusDebugger::onDataBitsChanged);
    connect(ui->stopBitsComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusDebugger::onStopBistChanged);
    connect(ui->customBaudRateCheckBox, &QCheckBox::clicked,
            this, &SAKModbusDebugger::onCustomBaudRateChanged);
    connect(ui->refreshPpushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::onInvokeRefresh);
}

void SAKModbusDebugger::initSignalsClient()
{
    connect(ui->clientTimeoutSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onClientTimeoutChanged);
    connect(ui->clientRepeatTimeSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onClientRepeatTimeChanged);
}

void SAKModbusDebugger::initSignalsServer()
{
    connect(ui->serverIsBusyCheckBox, &QCheckBox::clicked,
            this, &SAKModbusDebugger::onServerIsBusyChanged);
    connect(ui->serverJustListenCheckBox, &QCheckBox::clicked,
            this, &SAKModbusDebugger::onServerJustListenChanged);
    connect(ui->serverAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onServerAddressChanged);
}

void SAKModbusDebugger::initSignalsClientOperations()
{
    connect(ui->functionCodeComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusDebugger::onFunctionCodeChanged);
    connect(ui->targetAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onTargetAddressChanged);
    connect(ui->startAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onStartAddressChanged);
    connect(ui->addressNumberSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusDebugger::onAddressNumberChanged);
    connect(ui->readPushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::clientRead);
    connect(ui->writePushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::clientWrite);
    connect(ui->sendPushButton, &QPushButton::clicked,
            this, &SAKModbusDebugger::clientSend);
}

void SAKModbusDebugger::onErrorOccurred()
{
    outputMessage(mModbusDevice->errorString(), true, "", "error");
    if (mModbusDevice->error() == QModbusDevice::ConnectionError) {
        const QString msg = mModbusDevice->errorString();
        onInvokeClose();
        showMessageBox(tr("Error occured"), msg);
    }
#if 0
    SAKMDF->closeDevice(mModbusDevice);
    mModbusDevice = Q_NULLPTR;
#endif
}

void SAKModbusDebugger::onDeviceTypeChanged()
{
    int type = ui->devicesComboBox->currentData().toInt();
    bool isSerial =
        (type == ModbusDeviceRtuSerialClient
         || type == ModbusDeviceRtuSerialServer);
    bool isClient =
        (type == ModbusDeviceRtuSerialClient
         || type == ModbusDeviceTcpClient);

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
        ui->serverGroupBox->setHidden(true);
        ui->registersGroupBox->setHidden(true);
        ui->clientGroupBox->setHidden(false);
        ui->clientOperationsGroupBox->setHidden(false);
        ui->clientCustomCommandGroupBox->setHidden(false);
        ui->clientRegistersGroupBox->setHidden(false);
    } else {
        ui->clientCustomCommandGroupBox->setHidden(true);
        ui->clientRegistersGroupBox->setHidden(true);
        ui->clientGroupBox->setHidden(true);
        ui->clientOperationsGroupBox->setHidden(true);
        ui->serverGroupBox->setHidden(false);
        ui->registersGroupBox->setHidden(false);
    }

    mSettings->setValue(mSettingsKeyCtx.deviceIndex, type);
}

void SAKModbusDebugger::onInvokeClose()
{
    if (mModbusDevice) {
        if (mModbusDevice->inherits("QModbusDevice")) {
            mModbusDevice->disconnectDevice();
            mModbusDevice->deleteLater();
        }
    }

    mModbusDevice = Q_NULLPTR;
    updateUiState(false);
}

void SAKModbusDebugger::onInvokeOpen()
{
    ui->openDevicePushButton->setEnabled(false);
    if (mModbusDevice) {
        mModbusDevice->disconnectDevice();
        mModbusDevice->deleteLater();
        mModbusDevice = Q_NULLPTR;
    }

    int type = ui->devicesComboBox->currentData().toInt();
    if (type == ModbusDeviceRtuSerialClient
        || type == ModbusDeviceRtuSerialServer) {
        QString portName = ui->portNameComboBox->currentText();
        int parity = ui->parityComboBox->currentData().toInt();
        int baudRate = ui->baudRateComboBox->currentData().toInt();
        int dataBits = ui->dataBitsComboBox->currentData().toInt();
        int stopBits = ui->stopBitsComboBox->currentData().toInt();
        mModbusDevice = createRtuSerialDevice(
                    type, portName, parity, baudRate, dataBits, stopBits);
    } else {
        QString address = ui->addressComboBox->currentText();
        int port = ui->portSpinBox->value();
        mModbusDevice = createTcpDevice(type, address, port);
    }

    if (!mModbusDevice) {
        ui->openDevicePushButton->setEnabled(true);
        qCWarning(mLoggingCategory) << "Can not create modbus devices!";
        return;
    }

    if (isServer(mModbusDevice)) {
        if (!resetServerMap(mModbusDevice)) {
            qCWarning(mLoggingCategory) << "Can not reset server map!";
            return;
        }

        bool isBusy = ui->serverIsBusyCheckBox->isChecked();
        bool listenOnly = ui->serverJustListenCheckBox->isChecked();
        int address = ui->serverAddressSpinBox->value();
        setServerParameters(
            mModbusDevice, QModbusServer::DeviceBusy, isBusy);
        setServerParameters(
            mModbusDevice, QModbusServer::ListenOnlyMode, listenOnly);

        QModbusServer *server = qobject_cast<QModbusServer*>(mModbusDevice);
        server->setServerAddress(address);
        synchronizationRegister(server);
        connect(server, &QModbusServer::dataWritten, this,
                [=](QModbusDataUnit::RegisterType table, int address, int size){
            tableViewUpdateData(table, address, size);
        });
    }

    if (isClient(mModbusDevice)) {
        clientUpdateParameters();
    }

    connect(mModbusDevice, &QModbusDevice::errorOccurred,
            this, &SAKModbusDebugger::onErrorOccurred);
    bool opened = openDevice(mModbusDevice);
    updateUiState(opened);

    if (!opened) {
        QString errStr = modbuseDeviceErrorString(mModbusDevice);
        showMessageBox(tr("Can not open device"),
                       tr("Can not open device: ")
                           + errStr
                           + tr(". Please check the parameters and try again!"));
    }
}

void SAKModbusDebugger::onAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.address,
                        ui->addressComboBox->currentText());
}

void SAKModbusDebugger::onPortChanged()
{
    mSettings->setValue(mSettingsKeyCtx.port,
                        ui->portSpinBox->value());
}

void SAKModbusDebugger::onCustomAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.customAddress,
                        ui->customAddressCheckBox->isChecked());
    ui->addressComboBox->setEditable(ui->customAddressCheckBox->isChecked());
}

void SAKModbusDebugger::onPortNameChanged()
{
    mSettings->setValue(mSettingsKeyCtx.portName,
                        ui->portNameComboBox->currentText());
}

void SAKModbusDebugger::onParityChanged()
{
    mSettings->setValue(mSettingsKeyCtx.parity,
                        ui->parityComboBox->currentIndex());
}

void SAKModbusDebugger::onBaudRateChanged()
{
    mSettings->setValue(mSettingsKeyCtx.baudRate,
                        ui->baudRateComboBox->currentText());
}

void SAKModbusDebugger::onDataBitsChanged()
{
    mSettings->setValue(mSettingsKeyCtx.dataBits,
                        ui->dataBitsComboBox->currentIndex());
}

void SAKModbusDebugger::onStopBistChanged()
{
    mSettings->setValue(mSettingsKeyCtx.stopBits,
                        ui->stopBitsComboBox->currentIndex());
}

void SAKModbusDebugger::onInvokeRefresh()
{
    initComponentPortName();
}

void SAKModbusDebugger::onCustomBaudRateChanged()
{
    mSettings->setValue(mSettingsKeyCtx.customBaudRate,
                        ui->customBaudRateCheckBox->isChecked());
    ui->baudRateComboBox->setEditable(ui->customBaudRateCheckBox->isChecked());
}

void SAKModbusDebugger::onClientTimeoutChanged()
{
    mSettings->setValue(mSettingsKeyCtx.clientTimeout,
                        ui->clientTimeoutSpinBox->value());
    clientUpdateParameters();
}

void SAKModbusDebugger::onClientRepeatTimeChanged()
{
    mSettings->setValue(mSettingsKeyCtx.clientRepeatTime,
                        ui->clientRepeatTimeSpinBox->value());
    clientUpdateParameters();
}

void SAKModbusDebugger::onServerIsBusyChanged()
{
    mSettings->setValue(mSettingsKeyCtx.serverIsBusy,
                        ui->serverIsBusyCheckBox->isChecked());
    if (isServer(mModbusDevice)) {
        setServerParameters(mModbusDevice, QModbusServer::DeviceBusy,
                                    ui->serverIsBusyCheckBox->isChecked());
    }
}

void SAKModbusDebugger::onServerJustListenChanged()
{
    mSettings->setValue(mSettingsKeyCtx.serverJustListen,
                        ui->serverJustListenCheckBox->isChecked());
    setServerParameters(mModbusDevice, QModbusServer::ListenOnlyMode,
                        ui->serverJustListenCheckBox->isChecked());
}

void SAKModbusDebugger::onServerAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.serverAddress,
                        ui->serverAddressSpinBox->value());
    if (isServer(mModbusDevice)) {
        QModbusServer *server = qobject_cast<QModbusServer*>(mModbusDevice);
        server->setServerAddress(ui->serverAddressSpinBox->value());
    }
}

void SAKModbusDebugger::onFunctionCodeChanged()
{
    mSettings->setValue(mSettingsKeyCtx.functionCode,
                        ui->functionCodeComboBox->currentIndex());
    clientUpdateRWBtState();
}

void SAKModbusDebugger::onTargetAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.targetAddress,
                        ui->targetAddressSpinBox->value());
}

void SAKModbusDebugger::onStartAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.startAddress,
                        ui->startAddressSpinBox->value());
    clientUpdateTable();
}

void SAKModbusDebugger::onAddressNumberChanged()
{
    mSettings->setValue(mSettingsKeyCtx.addressNumber,
                        ui->addressNumberSpinBox->value());
    clientUpdateTable();
}

void SAKModbusDebugger::updateUiState(bool opened)
{
    ui->devicesComboBox->setEnabled(!opened);
    ui->closeDevicePushButton->setEnabled(opened);
    ui->openDevicePushButton->setEnabled(!opened);
    ui->networkGroupBox->setEnabled(!opened);
    ui->serialPortGroupBox->setEnabled(!opened);
#if 0
    ui->clientGroupBox->setEnabled(!opened);
    ui->serverGroupBox->setEnabled(!opened);
#endif
}

quint16 SAKModbusDebugger::calculateModbusCrc(const QByteArray &data)
{
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

quint16 SAKModbusDebugger::cookedRegisterString(QString text, int base)
{
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

void SAKModbusDebugger::synchronizationRegister(QModbusDevice *server)
{
    for (int i = 0; i < 4; i++) {
        QWidget *widget = ui->registerTabWidget->widget(i);
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
                setServerData(server, table, row, value);
            }
        }
    }
}

bool SAKModbusDebugger::writeSettingsArray(const QString &group, const QString &key,
                                   const QString &value, int index,
                                   int maxIndex)
{
    mSettings->beginWriteArray(group);
    for (int i = 0; i < maxIndex; i++) {
        mSettings->setArrayIndex(i);
        QString v = mSettings->value(key).toString();
        if (v == value) {
            mSettings->endArray();
            return false;
        }
    }

    mSettings->setArrayIndex(index);
    mSettings->setValue(key, value);
    mSettings->endArray();
    return true;
}

QModbusDataUnit::RegisterType SAKModbusDebugger::registerType(int type)
{
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

void SAKModbusDebugger::clientRead()
{
    if (!mModbusDevice) {
        showMessageBoxDeviceIsNotReady();
        return;
    }

    int registerType = ui->functionCodeComboBox->currentData().toInt();
    quint16 startAddress = ui->startAddressSpinBox->value();
    quint16 addressNumber = ui->addressNumberSpinBox->value();
    quint16 targetAddress = ui->targetAddressSpinBox->value();
    quint8 functionCode = this->clientFunctionCode();
    QModbusReply *reply = sendReadRequest(mModbusDevice, registerType,
                                          startAddress, addressNumber,
                                          targetAddress);

    if (isValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            outputModbusReply(reply, functionCode);

            if (reply->error() == QModbusDevice::NoError) {
                QJsonArray result = modbusReplyResult(reply);
                clientSetRegisterValue(result);
                reply->deleteLater();
            }
        });

        outputModbusRequestRead(targetAddress, functionCode,
                                startAddress, addressNumber);
    }
}

void SAKModbusDebugger::clientWrite()
{
    if (!mModbusDevice) {
        showMessageBoxDeviceIsNotReady();
        return;
    }

    int registerType = ui->functionCodeComboBox->currentData().toInt();
    int startAddress = ui->startAddressSpinBox->value();
    int targetAddress = ui->targetAddressSpinBox->value();
    int addressNumber = ui->addressNumberSpinBox->value();
    quint8 functionCode = this->clientFunctionCode();
    QJsonArray values = clientRegisterValue();
    QModbusReply *reply = sendWriteRequest(
        mModbusDevice, registerType, startAddress, values, targetAddress);
    if (isValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            outputModbusReply(reply, functionCode);
            reply->deleteLater();
        });

        outputModbusRequestWrite(targetAddress, functionCode, startAddress,
                                 addressNumber, values);
    }
}

void SAKModbusDebugger::clientSend()
{
    if (!mModbusDevice) {
        showMessageBoxDeviceIsNotReady();
        return;
    }

    quint8 serverAddress = ui->targetAddressSpinBox->value();
    QByteArray pdu = clientPdu();
    QByteArray data = pdu.length()
            ? QByteArray(pdu.data() + 1, pdu.length() - 1)
            : QByteArray();
    int functionCode = pdu.count() ? pdu.at(0) : QModbusDataUnit::Invalid;
    QModbusReply *reply = sendRawRequest(
        mModbusDevice, serverAddress, functionCode, data);

    qCWarning(mLoggingCategory) << "address:" << serverAddress
                                << "function code:" << functionCode
                                << "data:" << QString(pdu.toHex(' '));

    if (isValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            outputModbusReply(reply, functionCode);
            reply->deleteLater();
        });

        outputModbusRequestSend(serverAddress, pdu);
    }

    // Update settings data.
    int index = mSettings->value(mSettingsKeyCtx.sendHistoryIndex).toInt();
    bool ret = writeSettingsArray(
        mSettingsKeyCtx.sendHistory, mSettingsKeyCtx.pdu,
        QString(pdu.toHex(' ')), index, MAX_HISTORY_INDEX);
    if (ret) {
        if (index > ui->pduComboBox->count()) {
            ui->pduComboBox->addItem(QString(pdu.toHex(' ')));
        } else {
            ui->pduComboBox->insertItem(index, QString(pdu.toHex(' ')));
        }
        mSettings->setValue(mSettingsKeyCtx.sendHistoryIndex,
                            index + 1 > MAX_HISTORY_INDEX ? 0 : index + 1);
    }
}

void SAKModbusDebugger::clientUpdateTable()
{
    int number = ui->addressNumberSpinBox->value();
    int rowCount = mRegisterModel->rowCount();
    if (number > rowCount) {
        mRegisterModel->insertRows(rowCount, number - rowCount);
    } else if (number < rowCount) {
        mRegisterModel->removeRows(number, rowCount - number);
    }

    // Update address.
    int startAddress = ui->startAddressSpinBox->value();
    tableViewUpdateAddress(ui->registerTableView, startAddress);
}

void SAKModbusDebugger::clientSetRegisterValue(QJsonArray values)
{
    int columnCount = mRegisterModel->columnCount();
    for (int row = 0; row < values.count(); row++) {
        for (int column = 1; column < columnCount; column++) {
            int value = values.at(row).toInt();
            int base = (column < 4)
                           ? (column == 1 ? 2 : (column == 2 ? 10 : 16))
                           : (column == 4 ? 2 : (column == 5 ? 10 : 16));
            quint16 valueBE = qToBigEndian<quint16>(value);
            quint16 cookedValue =  column < 4 ? value : valueBE;

            QModelIndex index = mRegisterModel->index(row, column);
            QMap<int, QVariant> roles;
            int width = base == 2 ? 16 : (base == 10 ? 5 : 4);
            QString cookedStr = QString::number(cookedValue, base)
                                    .rightJustified(width, '0', true);
            roles.insert(Qt::DisplayRole, cookedStr);
            mRegisterModel->setItemData(index, roles);
        }
    }

    // Refresh the view, or the new value will not be show.
    ui->registerTableView->viewport()->update();
}

void SAKModbusDebugger::clientUpdateRWBtState()
{
    QStringList list = ui->functionCodeComboBox->currentText().split('-');
    int code = list.length() ? list.first().toInt(Q_NULLPTR, 16) : 0;
    bool isReadingOperation = false;
    if (code == 0x01 || code == 0x02 || code == 0x03 || code == 0x04) {
        isReadingOperation = true;
    }

    if (code == 0x05 || code == 0x06) {
        ui->addressNumberSpinBox->setValue(1);
    }

    ui->readPushButton->setEnabled(isReadingOperation);
    ui->writePushButton->setEnabled(!isReadingOperation);
}

void SAKModbusDebugger::clientUpdateParameters()
{
    if (isClient(mModbusDevice)) {
        int timeout = ui->clientTimeoutSpinBox->value();
        int repeatTime = ui->clientRepeatTimeSpinBox->value();
        setClientParameters(mModbusDevice, timeout, repeatTime);
    }
}

quint8 SAKModbusDebugger::clientFunctionCode()
{
    QStringList list = ui->functionCodeComboBox->currentText().split('-');
    if (list.length()) {
        return list.first().toInt();
    }

    return 0;
}

QJsonArray SAKModbusDebugger::clientRegisterValue()
{
    QJsonArray array;
    mRegisterModel = ui->registerTableView->model();
    for (int row = 0; row < mRegisterModel->rowCount(); row++) {
        // Get data from hex column.
        QStandardItem *item = mRegisterModel->item(row, 3);
        if (item) {
            QString text = item->text();
            array.append(text.toInt(Q_NULLPTR, 16));
        } else {
            array.append(0);
        }
    }

    return array;
}

QByteArray SAKModbusDebugger::clientPdu()
{
    QString text = ui->pduComboBox->currentText();
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

void SAKModbusDebugger::showMessageBox(const QString &title, const QString &msg)
{
    QMessageBox::warning(this, title, msg);
}

void SAKModbusDebugger::showMessageBoxDeviceIsNotReady()
{
    showMessageBox(tr("Device is not ready"),
                   tr("The device is not ready,"
                      " please open the device then try again."));
}

void SAKModbusDebugger::outputModbusReply(QModbusReply *reply, int functionCode)
{
    if (reply->error() != QModbusDevice::NoError) {
        outputMessage(reply->errorString(), true, "", "error");
        this->showMessageBox(tr("Error occured"),
                             reply->errorString());
        return;
    }

    int serverAddress = reply->serverAddress();
    QByteArray data = reply->rawResult().data();
    data.prepend(char(functionCode));
    data.prepend(char(serverAddress));
    quint16 crc = calculateModbusCrc(data);
    data.append(reinterpret_cast<char*>(&crc), 2);
    QString msg = QString::fromLatin1(data.toHex(' '));
    outputMessage(msg, false, RXCOLOR,
                  RXFLAG + QString::number(serverAddress));
}

void SAKModbusDebugger::outputModbusRequestSend(int serverAddress,
                                        const QByteArray &pdu)
{
    QByteArray tx;
    tx.append(char(serverAddress));
    tx.append(pdu);
    quint16 crc = calculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    outputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(serverAddress));
}

void SAKModbusDebugger::outputModbusRequestRead(int serverAddress, int functionCode,
                                        int startAddress, int addressNumber)
{
    QByteArray tx;
    quint16 startAddressBE = qToBigEndian<quint16>(startAddress);
    quint16 addressNumberBE = qToBigEndian<quint16>(addressNumber);
    tx.append(char(serverAddress));
    tx.append(char(functionCode));
    tx.append(reinterpret_cast<char*>(&startAddressBE), 2);
    tx.append(reinterpret_cast<char*>(&addressNumberBE), 2);
    quint16 crc = calculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    outputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(serverAddress));
}

void SAKModbusDebugger::outputModbusRequestWrite(int serverAddress, int functionCode,
                                         int startAddress, int addressNumber,
                                         QJsonArray values)
{
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
    quint16 crc = calculateModbusCrc(tx);
    tx.append(reinterpret_cast<char*>(&crc), 2);
    outputMessage(QString(tx.toHex(' ')), false, TXCOLOR,
                  TXFLAG + QString::number(serverAddress));
}

void SAKModbusDebugger::outputMessage(const QString &msg, bool isError,
                              const QString &color, const QString &flag)
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

    cookedMsg += flag.isEmpty()
            ? ""
            : QString("<font color=%1>[%2]</font> ").arg(cookedColor, flag);
    cookedMsg += msg;
    ui->textBrowser->append(cookedMsg);
}

QTableView *SAKModbusDebugger::tableView(QModbusDataUnit::RegisterType table)
{
    QWidget *tv = Q_NULLPTR;
    if (table == QModbusDataUnit::DiscreteInputs) {
        tv = ui->registerTabWidget->widget(0);
    } else if (table == QModbusDataUnit::Coils) {
        tv = ui->registerTabWidget->widget(1);
    } else if (table == QModbusDataUnit::InputRegisters) {
        tv = ui->registerTabWidget->widget(2);
    } else if (table == QModbusDataUnit::HoldingRegisters) {
        tv = ui->registerTabWidget->widget(3);
    } else {
        tv = ui->registerTabWidget->widget(3);
        qCWarning(mLoggingCategory)
                << "Get table view failed: unknow register type!";
    }

    return qobject_cast<QTableView*>(tv);
}

QTableView *SAKModbusDebugger::tableViewInit(int rowCount, QTableView *tv)
{
    if (!tv) {
        tv = new QTableView(this);
    }

    QHeaderView *hv = tv->horizontalHeader();
    QStandardItemModel *model = new QStandardItemModel(tv);
    QStringList labels =
        QStringList() << tr("Address") << tr("Bin") << tr("Dec") << tr("Hex")
                      << tr("Bin(BE)") << tr("Dec(BE)") << tr("Hex(BE)");
    model->setHorizontalHeaderLabels(labels);
    model->setColumnCount(7);
    model->setRowCount(rowCount);
    tv->setModel(model);
    tv->verticalHeader()->hide();
    tv->setItemDelegateForColumn(0, new ReadOnlyDelegate(tv));
    tv->setItemDelegateForColumn(4, new ReadOnlyDelegate(tv));
    tv->setItemDelegateForColumn(5, new ReadOnlyDelegate(tv));
    tv->setItemDelegateForColumn(6, new ReadOnlyDelegate(tv));
    tableViewUpdateAddress(tv, 0);

    for (int column = 1; column < model->columnCount(); column++) {
        hv->setSectionResizeMode(column, QHeaderView::Stretch);
    }

#if 0
    // Set the default value to 0.
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = model->index(row, 3);
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, "0");
        model->setItemData(index, roles);

        QStandardItem *item = model->item(row, 3);
        tableViewUpdateRow(item);
    }
#endif

    connect(model, &QStandardItemModel::itemChanged,
            this, &SAKModbusDebugger::tableViewUpdateRow);

    return tv;
}

QVector<quint16> SAKModbusDebugger::tableValues(QTableView *tv, int row,
                                                      int count)
{
    if (!tv) {
        qCWarning(mLoggingCategory) << __FUNCTION__
                                    << ": tv can not be null!";
        return QVector<quint16>();
    }

    auto *model = qobject_cast<QStandardItemModel*>(tv->model());
    QVector<quint16> values;
    for (int i = row; i < count; i++) {
        QStandardItem *item = model->item(i, 3);
        if (item) {
            QString text = item->text();
            values.append(text.toInt(Q_NULLPTR, 16));
        } else {
            values.append(0);
        }
    }

    return values;
}

void SAKModbusDebugger::tableViewUpdateAddress(QTableView *tv,
                                               int startAddress)
{
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

void SAKModbusDebugger::tableViewUpdateRow(QStandardItem *item)
{
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
    quint16 value = cookedRegisterString(text, base);
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

    if (isServer(mModbusDevice)) {
        int address = item->row();
        int currentIndex = ui->registerTabWidget->currentIndex();
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

        setServerData(mModbusDevice, table, address, value);
    }
}

void SAKModbusDebugger::tableViewUpdateData(int table, int address, int size)
{
    qCInfo(mLoggingCategory) << "Data written, table:" << table
                             << ", start address:" << address
                             << ", size:" << size << ".";
    QTableView *tv = tableView(registerType(table));
    QStandardItemModel *model =
            qobject_cast<QStandardItemModel*>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer*>(mModbusDevice);


    QJsonArray data = serverValues(server, table, address, size);
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

QModbusDevice *SAKModbusDebugger::createRtuSerialDevice(
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

QModbusDevice *SAKModbusDebugger::createTcpDevice(
        int deviceType, QString address, int port)
{
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

bool SAKModbusDebugger::isClient(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusClient")) {
            return true;
        }
    }

    return false;
}

bool SAKModbusDebugger::isServer(QModbusDevice *device)
{
    if (device && qobject_cast<QModbusServer*>(device)) {
        return true;
    }

    return false;
}

bool SAKModbusDebugger::resetServerMap(QModbusDevice *server)
{
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


bool SAKModbusDebugger::setClientParameters(
        QModbusDevice *device, int timeout, int numberOfRetries)
{
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

bool SAKModbusDebugger::setServerParameters(
        QModbusDevice *device, int option, QVariant value)
{
    if (device) {
        if (device->inherits("QModbusServer")) {
            QModbusServer *modbusServer = qobject_cast<QModbusServer*>(device);
            modbusServer->setValue(option, value);
            return true;
        }
    }

    return false;
}

bool SAKModbusDebugger::openDevice(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusDevice")) {
            return device->connectDevice();
        }
    }
    return false;
}

QString SAKModbusDebugger::modbuseDeviceErrorString(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusDevice")) {
            return device->errorString();
        } else {
            return QString("Invalid device object!");
        }
    }

    return QString("Invalid modbus device!");
}

bool SAKModbusDebugger::setServerData(
        QModbusDevice *server, QModbusDataUnit::RegisterType table,
        quint16 address, quint16 data)
{
    if (server) {
        if (server->inherits("QModbusServer")) {
            QModbusServer *modbusServer = qobject_cast<QModbusServer*>(server);
            return modbusServer->setData(table, address, data);
        }
    }

    return false;
}

QModbusReply *SAKModbusDebugger::sendReadRequest(
        QModbusDevice *device, int registerType, int startAddress,
        int size, int serverAddress)
{
    if (device && isClient(device)) {
        qCInfo(mLoggingCategory) << "register-type:" << registerType
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

QJsonArray SAKModbusDebugger::serverValues(
        QModbusServer *server, int registerType, int address, int size)
{
    QJsonArray values;
    if (server) {
        QModbusDataUnit::RegisterType table =
                static_cast<QModbusDataUnit::RegisterType>(registerType);
        for (int i = address; i < size; i++) {
            quint16 value;
            if (server->data(table, i, &value)) {
                values.append(value);
            } else {
                qCWarning(mLoggingCategory) << "Parameters error!";
                break;
            }
        }
    } else {
        qCWarning(mLoggingCategory) << "Can not get values from null object!";
    }

    return values;
}

QModbusReply *SAKModbusDebugger::sendWriteRequest(
        QModbusDevice *device, int registerType, int startAddress,
        QJsonArray values, int serverAddress)
{
    if (device && isClient(device)) {
        QModbusDataUnit::RegisterType cookedRegisterType
            = static_cast<QModbusDataUnit::RegisterType>(registerType);
        QVector<quint16> registerValues;
        for (int i = 0; i < values.count(); i++) {
            registerValues.append(values.at(i).toInt());
        }

        QModbusDataUnit dataUnit(cookedRegisterType,
                                 startAddress, registerValues);
        if (dataUnit.isValid()) {
            qCInfo(mLoggingCategory) << "register-type:" << registerType
                                     << " start-address:" << startAddress
                                     << " serverAddress:" << serverAddress
                                     << " values:" << values;

            QModbusClient *client = qobject_cast<QModbusClient*>(device);
            QModbusReply *reply = client->sendWriteRequest(dataUnit,
                                                           serverAddress);
            return reply;
        } else {
            qCWarning(mLoggingCategory) << "Unvalid data unit!";
        }
    }

    return Q_NULLPTR;
}

bool SAKModbusDebugger::isValidModbusReply(QVariant reply)
{
    if (reply.canConvert<QModbusReply*>()) {
        QModbusReply *modbusReply = reply.value<QModbusReply*>();
        if (!modbusReply) {
            qCWarning(mLoggingCategory) << "QModbusReply is null!";
            return false;
        } else if (modbusReply->isFinished()) {
            qCWarning(mLoggingCategory) << "QModbusReply is finished!";
            return false;
        } else {
            return true;
        }
    }

    return false;
}

QJsonArray SAKModbusDebugger::modbusReplyResult(QModbusReply *reply)
{
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

QModbusReply *SAKModbusDebugger::sendRawRequest(
        QModbusDevice *device, int serverAddress, int functionCode,
        const QByteArray &data)
{
    auto fc = static_cast<QModbusPdu::FunctionCode>(functionCode);
    QModbusRequest request(fc, data);
    if (device && isClient(device)) {
        QModbusClient *client = qobject_cast<QModbusClient*>(device);
        return client->sendRawRequest(request, serverAddress);
    }

    return Q_NULLPTR;
}
