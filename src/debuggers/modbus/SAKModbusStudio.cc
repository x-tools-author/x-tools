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

#include "SAKModbusStudio.hh"
#include "SAKModbusDeviceFactory.hh"

#include "ui_SAKModbusStudio.h"

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

SAKModbusStudio::SAKModbusStudio(QSettings *settings, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKModbusStudio)
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

SAKModbusStudio::SAKModbusStudio(QSettings *settings,
                     const QString settingsGroup,
                     QSqlDatabase *sqlDatabase,
                     QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SAKModbusStudio)
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

SAKModbusStudio::~SAKModbusStudio()
{
    delete ui;
}

void SAKModbusStudio::initComponents()
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

void SAKModbusStudio::initComponentDevices()
{
    ui->devicesComboBox->addItem(
        tr("RtuClient"),
        SAKModbusDeviceFactory::ModbusDeviceRtuSerialClient);
    ui->devicesComboBox->addItem(
        tr("RtuServer"),
        SAKModbusDeviceFactory::ModbusDeviceRtuSerialServer);
    ui->devicesComboBox->addItem(
        tr("TcpClient"),
        SAKModbusDeviceFactory::ModbusDeviceTcpClient);
    ui->devicesComboBox->addItem(
        tr("TcpServer"),
        SAKModbusDeviceFactory::ModbusDeviceTcpServer);
}

void SAKModbusStudio::initComponentAddress()
{
    ui->addressComboBox->clear();
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ui->addressComboBox->addItem(address.toString());
        }
    }
}

void SAKModbusStudio::initComponentPortName()
{
    ui->portNameComboBox->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo &info : infos) {
        ui->portNameComboBox->addItem(info.portName());
    }
}

void SAKModbusStudio::initComponnetBaudRate()
{
    ui->baudRateComboBox->clear();
    QList<qint32> bds = QSerialPortInfo::standardBaudRates();
    for (qint32 &bd : bds) {
        ui->baudRateComboBox->addItem(QString::number(bd), bd);
    }
}

void SAKModbusStudio::initComponnetDataBits()
{
    ui->dataBitsComboBox->clear();
    ui->dataBitsComboBox->addItem("8", QSerialPort::Data8);
    ui->dataBitsComboBox->addItem("7", QSerialPort::Data7);
    ui->dataBitsComboBox->addItem("6", QSerialPort::Data6);
    ui->dataBitsComboBox->addItem("5", QSerialPort::Data5);
}

void SAKModbusStudio::initComponnetStopBits()
{
    ui->stopBitsComboBox->clear();
    ui->stopBitsComboBox->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsComboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsComboBox->addItem("2", QSerialPort::TwoStop);
}

void SAKModbusStudio::initComponnetParity()
{
    ui->parityComboBox->clear();
    ui->parityComboBox->addItem(tr("NoParity"), QSerialPort::NoParity);
    ui->parityComboBox->addItem(tr("EvenParity"), QSerialPort::EvenParity);
    ui->parityComboBox->addItem(tr("OddParity"), QSerialPort::OddParity);
    ui->parityComboBox->addItem(tr("SpaceParity"), QSerialPort::SpaceParity);
    ui->parityComboBox->addItem(tr("MarkParity"), QSerialPort::MarkParity);
}

void SAKModbusStudio::initComponentFunctionCode()
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

void SAKModbusStudio::initComponentRegisterTableView()
{
    QTableView *tv = tableViewInit(1, ui->registerTableView);
    mRegisterModel = reinterpret_cast<QStandardItemModel*>(tv->model());
}

void SAKModbusStudio::initComponentInput()
{
    QRegularExpression re("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QLineEdit *le = ui->pduComboBox->lineEdit();
    QRegularExpressionValidator *rev = new QRegularExpressionValidator(re, le);
    le->setValidator(rev);
    le->clear();
}

void SAKModbusStudio::initComponentRegisterTabWidget()
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

void SAKModbusStudio::initSettings()
{
    initSettingsDevice();
    initSettingsNetwork();
    initSettingsSerialPort();
    initSettingsClient();
    initSettingsServer();
    initSettingsClientOperations();
    initSettingsInput();
}

void SAKModbusStudio::initSettingsDevice()
{
    int deviceIndex = mSettings->value(mSettingsKeyCtx.deviceIndex).toInt();
    if (deviceIndex >= 0 && deviceIndex < ui->devicesComboBox->count()) {
        ui->devicesComboBox->setCurrentIndex(deviceIndex);
    }
}

void SAKModbusStudio::initSettingsNetwork()
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

void SAKModbusStudio::initSettingsSerialPort()
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

void SAKModbusStudio::initSettingsClient()
{
    int timeout = mSettings->value(mSettingsKeyCtx.clientTimeout).toInt();
    ui->clientTimeoutSpinBox->setValue(timeout < 100 ? 100 : timeout);

    int repeatTimes =
        mSettings->value(mSettingsKeyCtx.clientRepeatTime).toInt();
    ui->clientRepeatTimeSpinBox->setValue(repeatTimes);
}

void SAKModbusStudio::initSettingsServer()
{
    bool isBusy = mSettings->value(mSettingsKeyCtx.serverIsBusy).toBool();
    ui->serverIsBusyCheckBox->setChecked(isBusy);

    bool justListen = mSettings->value(mSettingsKeyCtx.serverJustListen).toBool();
    ui->serverJustListenCheckBox->setChecked(justListen);

    int address = mSettings->value(mSettingsKeyCtx.serverAddress).toInt();
    ui->serverAddressSpinBox->setValue(address);
}

void SAKModbusStudio::initSettingsClientOperations()
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

void SAKModbusStudio::initSettingsInput()
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

void SAKModbusStudio::initSignals()
{
    initSignalsDevice();
    initSignalsNetworking();
    initSignalsSerialPort();
    initSignalsClient();
    initSignalsServer();
    initSignalsClientOperations();
}

void SAKModbusStudio::initSignalsDevice()
{
    connect(ui->devicesComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusStudio::onDeviceTypeChanged);
    connect(ui->openDevicePushButton, &QPushButton::clicked,
            this, &SAKModbusStudio::onInvokeOpen);
    connect(ui->closeDevicePushButton, &QPushButton::clicked,
            this, &SAKModbusStudio::onInvokeClose);

}

void SAKModbusStudio::initSignalsNetworking()
{
    connect(ui->addressComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusStudio::onAddressChanged);
    connect(ui->portSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onPortChanged);
    connect(ui->customAddressCheckBox, &QCheckBox::clicked,
            this, &SAKModbusStudio::onCustomAddressChanged);

}

void SAKModbusStudio::initSignalsSerialPort()
{
    connect(ui->portNameComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusStudio::onPortNameChanged);
    connect(ui->parityComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusStudio::onParityChanged);
    connect(ui->baudRateComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusStudio::onBaudRateChanged);
    connect(ui->dataBitsComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusStudio::onDataBitsChanged);
    connect(ui->stopBitsComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SAKModbusStudio::onStopBistChanged);
    connect(ui->customBaudRateCheckBox, &QCheckBox::clicked,
            this, &SAKModbusStudio::onCustomBaudRateChanged);
    connect(ui->refreshPpushButton, &QPushButton::clicked,
            this, &SAKModbusStudio::onInvokeRefresh);
}

void SAKModbusStudio::initSignalsClient()
{
    connect(ui->clientTimeoutSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onClientTimeoutChanged);
    connect(ui->clientRepeatTimeSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onClientRepeatTimeChanged);
}

void SAKModbusStudio::initSignalsServer()
{
    connect(ui->serverIsBusyCheckBox, &QCheckBox::clicked,
            this, &SAKModbusStudio::onServerIsBusyChanged);
    connect(ui->serverJustListenCheckBox, &QCheckBox::clicked,
            this, &SAKModbusStudio::onServerJustListenChanged);
    connect(ui->serverAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onServerAddressChanged);
}

void SAKModbusStudio::initSignalsClientOperations()
{
    connect(ui->functionCodeComboBox, &QComboBox::currentTextChanged,
            this, &SAKModbusStudio::onFunctionCodeChanged);
    connect(ui->targetAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onTargetAddressChanged);
    connect(ui->startAddressSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onStartAddressChanged);
    connect(ui->addressNumberSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKModbusStudio::onAddressNumberChanged);
    connect(ui->readPushButton, &QPushButton::clicked,
            this, &SAKModbusStudio::clientRead);
    connect(ui->writePushButton, &QPushButton::clicked,
            this, &SAKModbusStudio::clientWrite);
    connect(ui->sendPushButton, &QPushButton::clicked,
            this, &SAKModbusStudio::clientSend);
}

void SAKModbusStudio::onErrorOccurred()
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

void SAKModbusStudio::onDeviceTypeChanged()
{
    int type = ui->devicesComboBox->currentData().toInt();
    bool isSerial =
        (type == SAKModbusDeviceFactory::ModbusDeviceRtuSerialClient
         || type == SAKModbusDeviceFactory::ModbusDeviceRtuSerialServer);
    bool isClient =
        (type == SAKModbusDeviceFactory::ModbusDeviceRtuSerialClient
         || type == SAKModbusDeviceFactory::ModbusDeviceTcpClient);

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

void SAKModbusStudio::onInvokeClose()
{
    SAKMDF->closeDevice(mModbusDevice);
    mModbusDevice = Q_NULLPTR;
    updateUiState(false);
}

void SAKModbusStudio::onInvokeOpen()
{
    ui->openDevicePushButton->setEnabled(false);
    if (mModbusDevice) {
        mModbusDevice->disconnectDevice();
        mModbusDevice->deleteLater();
        mModbusDevice = Q_NULLPTR;
    }

    int type = ui->devicesComboBox->currentData().toInt();
    if (type == SAKModbusDeviceFactory::ModbusDeviceRtuSerialClient
        || type == SAKModbusDeviceFactory::ModbusDeviceRtuSerialServer) {
        QString portName = ui->portNameComboBox->currentText();
        int parity = ui->parityComboBox->currentData().toInt();
        int baudRate = ui->baudRateComboBox->currentData().toInt();
        int dataBits = ui->dataBitsComboBox->currentData().toInt();
        int stopBits = ui->stopBitsComboBox->currentData().toInt();
        mModbusDevice =
            SAKMDF->createRtuSerialDevice(type, portName, parity, baudRate,
                                          dataBits, stopBits);
    } else {
        QString address = ui->addressComboBox->currentText();
        int port = ui->portSpinBox->value();
        mModbusDevice = SAKMDF->createTcpDevice(type, address, port);
    }

    if (!mModbusDevice) {
        ui->openDevicePushButton->setEnabled(true);
        qCWarning(mLoggingCategory) << "Can not create modbus devices!";
        return;
    }

    if (SAKMDF->isServer(mModbusDevice)) {
        SAKMDF->resetServerMap(QVariant::fromValue(mModbusDevice));

        bool isBusy = ui->serverIsBusyCheckBox->isChecked();
        bool listenOnly = ui->serverJustListenCheckBox->isChecked();
        int address = ui->serverAddressSpinBox->value();
        SAKMDF->setServerParameters(
            mModbusDevice, QModbusServer::DeviceBusy, isBusy);
        SAKMDF->setServerParameters(
            mModbusDevice, QModbusServer::ListenOnlyMode, listenOnly);

        QModbusServer *server = qobject_cast<QModbusServer*>(mModbusDevice);
        server->setServerAddress(address);
        synchronizationRegister(server);
        connect(server, &QModbusServer::dataWritten,
                this, &SAKModbusStudio::tableViewUpdateData);
    }

    if (SAKMDF->isClient(mModbusDevice)) {
        clientUpdateParameters();
    }

    connect(mModbusDevice, &QModbusDevice::errorOccurred,
            this, &SAKModbusStudio::onErrorOccurred);
    bool opened = SAKMDF->openDevice(mModbusDevice);
    updateUiState(opened);

    if (!opened) {
        QString errStr = SAKMDF->modbuseDeviceErrorString(mModbusDevice);
        showMessageBox(tr("Can not open device"),
                       tr("Can not open device: ")
                           + errStr
                           + tr(". Please check the parameters and try again!"));
    }
}

void SAKModbusStudio::onAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.address,
                        ui->addressComboBox->currentText());
}

void SAKModbusStudio::onPortChanged()
{
    mSettings->setValue(mSettingsKeyCtx.port,
                        ui->portSpinBox->value());
}

void SAKModbusStudio::onCustomAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.customAddress,
                        ui->customAddressCheckBox->isChecked());
    ui->addressComboBox->setEditable(ui->customAddressCheckBox->isChecked());
}

void SAKModbusStudio::onPortNameChanged()
{
    mSettings->setValue(mSettingsKeyCtx.portName,
                        ui->portNameComboBox->currentText());
}

void SAKModbusStudio::onParityChanged()
{
    mSettings->setValue(mSettingsKeyCtx.parity,
                        ui->parityComboBox->currentIndex());
}

void SAKModbusStudio::onBaudRateChanged()
{
    mSettings->setValue(mSettingsKeyCtx.baudRate,
                        ui->baudRateComboBox->currentText());
}

void SAKModbusStudio::onDataBitsChanged()
{
    mSettings->setValue(mSettingsKeyCtx.dataBits,
                        ui->dataBitsComboBox->currentIndex());
}

void SAKModbusStudio::onStopBistChanged()
{
    mSettings->setValue(mSettingsKeyCtx.stopBits,
                        ui->stopBitsComboBox->currentIndex());
}

void SAKModbusStudio::onInvokeRefresh()
{
    initComponentPortName();
}

void SAKModbusStudio::onCustomBaudRateChanged()
{
    mSettings->setValue(mSettingsKeyCtx.customBaudRate,
                        ui->customBaudRateCheckBox->isChecked());
    ui->baudRateComboBox->setEditable(ui->customBaudRateCheckBox->isChecked());
}

void SAKModbusStudio::onClientTimeoutChanged()
{
    mSettings->setValue(mSettingsKeyCtx.clientTimeout,
                        ui->clientTimeoutSpinBox->value());
    clientUpdateParameters();
}

void SAKModbusStudio::onClientRepeatTimeChanged()
{
    mSettings->setValue(mSettingsKeyCtx.clientRepeatTime,
                        ui->clientRepeatTimeSpinBox->value());
    clientUpdateParameters();
}

void SAKModbusStudio::onServerIsBusyChanged()
{
    mSettings->setValue(mSettingsKeyCtx.serverIsBusy,
                        ui->serverIsBusyCheckBox->isChecked());
    if (SAKMDF->isServer(mModbusDevice)) {
        SAKMDF->setServerParameters(mModbusDevice, QModbusServer::DeviceBusy,
                                    ui->serverIsBusyCheckBox->isChecked());
    }
}

void SAKModbusStudio::onServerJustListenChanged()
{
    mSettings->setValue(mSettingsKeyCtx.serverJustListen,
                        ui->serverJustListenCheckBox->isChecked());
    SAKMDF->setServerParameters(mModbusDevice, QModbusServer::ListenOnlyMode,
                                ui->serverJustListenCheckBox->isChecked());
}

void SAKModbusStudio::onServerAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.serverAddress,
                        ui->serverAddressSpinBox->value());
    if (SAKMDF->isServer(mModbusDevice)) {
        QModbusServer *server = qobject_cast<QModbusServer*>(mModbusDevice);
        server->setServerAddress(ui->serverAddressSpinBox->value());
    }
}

void SAKModbusStudio::onFunctionCodeChanged()
{
    mSettings->setValue(mSettingsKeyCtx.functionCode,
                        ui->functionCodeComboBox->currentIndex());
    clientUpdateRWBtState();
}

void SAKModbusStudio::onTargetAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.targetAddress,
                        ui->targetAddressSpinBox->value());
}

void SAKModbusStudio::onStartAddressChanged()
{
    mSettings->setValue(mSettingsKeyCtx.startAddress,
                        ui->startAddressSpinBox->value());
    clientUpdateTable();
}

void SAKModbusStudio::onAddressNumberChanged()
{
    mSettings->setValue(mSettingsKeyCtx.addressNumber,
                        ui->addressNumberSpinBox->value());
    clientUpdateTable();
}

void SAKModbusStudio::updateUiState(bool opened)
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

quint16 SAKModbusStudio::calculateModbusCrc(const QByteArray &data)
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

quint16 SAKModbusStudio::cookedRegisterString(QString text, int base)
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

void SAKModbusStudio::synchronizationRegister(QModbusDevice *server)
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
                SAKMDF->setServerData(server, table, row, value);
            }
        }
    }
}

bool SAKModbusStudio::writeSettingsArray(const QString &group, const QString &key,
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

QModbusDataUnit::RegisterType SAKModbusStudio::registerType(int type)
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

void SAKModbusStudio::clientRead()
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
    QModbusReply *reply = SAKMDF->sendReadRequest(mModbusDevice, registerType,
                                                  startAddress, addressNumber,
                                                  targetAddress);

    if (SAKMDF->isValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            outputModbusReply(reply, functionCode);

            if (reply->error() == QModbusDevice::NoError) {
                QJsonArray result = SAKMDF->modbusReplyResult(reply);
                clientSetRegisterValue(result);
                reply->deleteLater();
            }
        });

        outputModbusRequestRead(targetAddress, functionCode,
                                startAddress, addressNumber);
    }
}

void SAKModbusStudio::clientWrite()
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
    QModbusReply *reply = SAKMDF->sendWriteRequest(
        mModbusDevice, registerType, startAddress, values, targetAddress);
    if (SAKMDF->isValidModbusReply(QVariant::fromValue(reply))) {
        connect(reply, &QModbusReply::finished, this, [=](){
            outputModbusReply(reply, functionCode);
            reply->deleteLater();
        });

        outputModbusRequestWrite(targetAddress, functionCode, startAddress,
                                 addressNumber, values);
    }
}

void SAKModbusStudio::clientSend()
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
    QModbusReply *reply = SAKMDF->sendRawRequest(
        mModbusDevice, serverAddress, functionCode, data);

    qCWarning(mLoggingCategory) << "address:" << serverAddress
                                << "function code:" << functionCode
                                << "data:" << QString(pdu.toHex(' '));

    if (SAKMDF->isValidModbusReply(QVariant::fromValue(reply))) {
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

void SAKModbusStudio::clientUpdateTable()
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

void SAKModbusStudio::clientSetRegisterValue(QJsonArray values)
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

void SAKModbusStudio::clientUpdateRWBtState()
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

void SAKModbusStudio::clientUpdateParameters()
{
    if (SAKMDF->isClient(mModbusDevice)) {
        int timeout = ui->clientTimeoutSpinBox->value();
        int repeatTime = ui->clientRepeatTimeSpinBox->value();
        SAKMDF->setClientParameters(mModbusDevice, timeout, repeatTime);
    }
}

quint8 SAKModbusStudio::clientFunctionCode()
{
    QStringList list = ui->functionCodeComboBox->currentText().split('-');
    if (list.length()) {
        return list.first().toInt();
    }

    return 0;
}

QJsonArray SAKModbusStudio::clientRegisterValue()
{
    QJsonArray array;
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

QByteArray SAKModbusStudio::clientPdu()
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

void SAKModbusStudio::showMessageBox(const QString &title, const QString &msg)
{
    QMessageBox::warning(this, title, msg);
}

void SAKModbusStudio::showMessageBoxDeviceIsNotReady()
{
    showMessageBox(tr("Device is not ready"),
                   tr("The device is not ready,"
                      " please open the device then try again."));
}

void SAKModbusStudio::outputModbusReply(QModbusReply *reply, int functionCode)
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

void SAKModbusStudio::outputModbusRequestSend(int serverAddress,
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

void SAKModbusStudio::outputModbusRequestRead(int serverAddress, int functionCode,
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

void SAKModbusStudio::outputModbusRequestWrite(int serverAddress, int functionCode,
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

void SAKModbusStudio::outputMessage(const QString &msg, bool isError,
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

QTableView *SAKModbusStudio::tableView(QModbusDataUnit::RegisterType table)
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

QTableView *SAKModbusStudio::tableViewInit(int rowCount, QTableView *tv)
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
            this, &SAKModbusStudio::tableViewUpdateRow);

    return tv;
}

QVector<quint16> SAKModbusStudio::tableValues(QTableView *tv, int row,
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

void SAKModbusStudio::tableViewUpdateAddress(QTableView *tv,
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

void SAKModbusStudio::tableViewUpdateRow(QStandardItem *item)
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

    if (SAKMDF->isServer(mModbusDevice)) {
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

        SAKMDF->setServerData(mModbusDevice, table, address, value);
    }
}

void SAKModbusStudio::tableViewUpdateData(int table, int address, int size)
{
    qCInfo(mLoggingCategory) << "Data written, table:" << table
                             << ", start address:" << address
                             << ", size:" << size << ".";
    QTableView *tv = tableView(registerType(table));
    QStandardItemModel *model =
            qobject_cast<QStandardItemModel*>(tv->model());
    QModbusServer *server = qobject_cast<QModbusServer*>(mModbusDevice);


    QJsonArray data = SAKMDF->serverValues(server, table, address, size);
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
