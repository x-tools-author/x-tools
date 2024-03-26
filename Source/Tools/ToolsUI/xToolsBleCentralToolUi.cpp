/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBleCentralToolUi.h"
#include "ui_xToolsBleCentralToolUi.h"

#include <QGridLayout>
#include <QLabel>
#include <QLowEnergyCharacteristic>
#include <QMenu>
#include <QWidget>
#include <QWidgetAction>

#include "xToolsLineEdit.h"
#include "xToolsSpinBox.h"

#define SAK_CB_I_C &QComboBox::currentIndexChanged

xToolsBleCentralToolUi::xToolsBleCentralToolUi(QWidget* parent)
    : xToolsCommunicationToolUi{parent}
    , ui(new Ui::xToolsBleCentralToolUi)
    , mBleTool(Q_NULLPTR)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->pushButtonScan,
            &QPushButton::clicked,
            this,
            &xToolsBleCentralToolUi::onPushButtonScanClicked);
    connect(ui->comboBoxDevices,
            static_cast<void (QComboBox::*)(int)>(SAK_CB_I_C),
            this,
            &xToolsBleCentralToolUi::onComboBoxDevicesActived);
    connect(ui->comboBoxServices,
            static_cast<void (QComboBox::*)(int)>(SAK_CB_I_C),
            this,
            &xToolsBleCentralToolUi::onComboBoxServicesCurrentIndexChanged);
    connect(ui->comboBoxCharacteristics,
            static_cast<void (QComboBox::*)(int)>(SAK_CB_I_C),
            this,
            &xToolsBleCentralToolUi::onComboBoxCharacteristicsActived);
    connect(ui->comboBoxWriteWay,
            static_cast<void (QComboBox::*)(int)>(SAK_CB_I_C),
            this,
            &xToolsBleCentralToolUi::onComboBoxWriteWayCurrentIndexChanged);
    connect(ui->pushButtonNotify,
            &QPushButton::clicked,
            this,
            &xToolsBleCentralToolUi::onPushButtonNotifyClicked);
    connect(ui->pushButtonRead,
            &QPushButton::clicked,
            this,
            &xToolsBleCentralToolUi::onPushButtonReadClicked);
    connect(ui->comboBoxDevices, &xToolsBleDeviceInfoComboBox::finished, this, [=]() {
        ui->pushButtonScan->setText(tr("Scan"));
        ui->pushButtonScan->setEnabled(true);
    });
    connect(ui->comboBoxDevices, &xToolsBleDeviceInfoComboBox::started, this, [=]() {
        ui->pushButtonScan->setText(tr("Stop"));
        ui->pushButtonScan->setEnabled(true);
    });

    ui->labelWriteWay->setVisible(false);
    ui->comboBoxWriteWay->setVisible(false);
    ui->pushButtonNotify->setVisible(false);
    ui->pushButtonRead->setVisible(false);
    ui->labelUnsupported->setVisible(false);
    ui->labelUnsupported->setStyleSheet("QLabel{color:red}");
}

xToolsBleCentralToolUi::~xToolsBleCentralToolUi()
{
    delete ui;
}

void xToolsBleCentralToolUi::onBaseToolUiInitialized(xToolsBaseTool* tool, const QString& settingsGroup)
{
    xToolsCommunicationToolUi::onBaseToolUiInitialized(tool, settingsGroup);

    mBleTool = qobject_cast<xToolsBleCentralTool*>(mTool);
    if (!mBleTool) {
        QByteArray msg("invalid SAKBleCentralTool tool");
        qWarning() << QString::fromLatin1(msg);
        Q_ASSERT_X(false, __FUNCTION__, msg.data());
        return;
    }

    onComboBoxWriteWayCurrentIndexChanged();
    connect(mBleTool,
            &xToolsBleCentralTool::descriptorWritten,
            this,
            &xToolsBleCentralToolUi::onDescriptorWritten);

    initSettingsMenu(settingsGroup);
}

void xToolsBleCentralToolUi::onIsWorkingChanged(bool isWorking)
{
    if (!isWorking) {
        ui->progressBar->hide();
    }
}

void xToolsBleCentralToolUi::initSettingsMenu(const QString& settingsGroup)
{
    QWidget* w = new QWidget(this);
    QGridLayout* gl = new QGridLayout();
    w->setLayout(gl);

    int rowIndex = 0;
    gl->addWidget(new QLabel(tr("Timeout interval(S)"), w), rowIndex, 0, 1, 1);
    xToolsSpinBox* sp = new xToolsSpinBox(w);
    sp->setMinimum(10);
    sp->setMaximum(120);
    sp->setValue(120);
    sp->setGroupKey(settingsGroup, "timeoutInterval");
    gl->addWidget(sp, rowIndex, 1, 1, 1);
    connect(sp,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            [=](int v) { ui->comboBoxDevices->setTimeoutInterval(v); });

    rowIndex += 1;
    gl->addWidget(new QLabel(tr("Name filtter"), w), rowIndex, 0, 1, 1);
    xToolsLineEdit* le = new xToolsLineEdit(w);
    le->setGroupKey(settingsGroup, "nameFiltter");
    gl->addWidget(le, rowIndex, 1, 1, 1);
    connect(le, &xToolsLineEdit::textChanged, this, [=](const QString& text) {
        ui->comboBoxDevices->setNameFiltter(text);
    });

    rowIndex += 1;
    QMenu* menu = new QMenu(this);
    QPushButton* bt = new QPushButton(tr("OK"));
    connect(bt, &QPushButton::clicked, menu, &QMenu::close);
    gl->addWidget(bt, rowIndex, 1, 1, 1);

    QWidgetAction* a = new QWidgetAction(this);
    a->setDefaultWidget(w);

    menu->addAction(a);
    ui->pushButtonSettings->setMenu(menu);

    connect(mBleTool,
            &xToolsBleCentralTool::serviceDiscoveryStarted,
            this,
            &xToolsBleCentralToolUi::onServiceDiscoveryStarted);
    connect(mBleTool,
            &xToolsBleCentralTool::serviceDiscoveryFinished,
            this,
            &xToolsBleCentralToolUi::onServiceDiscoveryFinished);

    int timeoutInterval = sp->value();
    QString nameFiltter = le->text().trimmed();
    ui->comboBoxDevices->setTimeoutInterval(timeoutInterval);
    ui->comboBoxDevices->setNameFiltter(nameFiltter);

    onComboBoxDevicesActived();
}

void xToolsBleCentralToolUi::onServiceDiscoveryStarted()
{
    ui->progressBar->show();
}

void xToolsBleCentralToolUi::onServiceDiscoveryFinished()
{
    ui->comboBoxServices->clear();
    auto services = mBleTool->services();
    for (auto& service : services) {
        auto cookedSerivce = service.value<QLowEnergyService*>();
        ui->comboBoxServices->addItem(cookedSerivce->serviceName(), service);
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
        connect(cookedSerivce,
                &QLowEnergyService::stateChanged,
                this,
                [=](QLowEnergyService::ServiceState state) {
                    if (state == QLowEnergyService::RemoteServiceDiscovered) {
                        onComboBoxServicesCurrentIndexChanged();
                    }
                });
#endif
    }
    ui->progressBar->hide();
}

void xToolsBleCentralToolUi::onDescriptorWritten(const QLowEnergyDescriptor& descriptor,
                                              const QByteArray& newValue)
{
    Q_UNUSED(descriptor)
    Q_UNUSED(newValue)
    onComboBoxCharacteristicsActived();
}

void xToolsBleCentralToolUi::onPushButtonScanClicked()
{
    ui->pushButtonScan->setEnabled(false);
    if (ui->comboBoxDevices->isActive()) {
        ui->comboBoxDevices->stopDiscover();
    } else {
        ui->comboBoxDevices->startDiscover();
    }
}

void xToolsBleCentralToolUi::onComboBoxDevicesActived()
{
    QVariant data = ui->comboBoxDevices->currentData();
    mBleTool->setInfo(data);
}

void xToolsBleCentralToolUi::onComboBoxServicesCurrentIndexChanged()
{
    mBleTool->setServiceIndex(ui->comboBoxServices->currentIndex());
    auto service = ui->comboBoxServices->currentData();
    auto cookedSerivce = service.value<QLowEnergyService*>();
    auto chs = cookedSerivce->characteristics();
    ui->comboBoxCharacteristics->clear();
    for (int i = 0; i < chs.count(); i++) {
        auto ch = chs.at(i);
        QVariant var = QVariant::fromValue<QLowEnergyCharacteristic>(ch);
        QString name = ch.name();
        name = name.isEmpty() ? tr("Characteristics%1").arg(i + 1) : name;
        ui->comboBoxCharacteristics->addItem(name, var);
    }
}

void xToolsBleCentralToolUi::onComboBoxCharacteristicsActived()
{
    int index = ui->comboBoxCharacteristics->currentIndex();
    mBleTool->setCharacteristicIndex(index);

    QVariant ch = ui->comboBoxCharacteristics->currentData();
    bool writeFlag = mBleTool->hasFlag(ch, QLowEnergyCharacteristic::Write);
    int writeNoResponse = QLowEnergyCharacteristic::WriteNoResponse;
    bool writeNoResponseFlag = mBleTool->hasFlag(ch, writeNoResponse);
    ui->labelWriteWay->setVisible(writeFlag);
    ui->comboBoxWriteWay->setVisible(writeNoResponseFlag);

    bool readFlag = mBleTool->hasFlag(ch, QLowEnergyCharacteristic::Read);
    bool notifyFlag = mBleTool->hasFlag(ch, QLowEnergyCharacteristic::Notify);
    ui->pushButtonNotify->setVisible(notifyFlag);
    ui->pushButtonRead->setVisible(readFlag);
    bool supported = writeFlag || writeNoResponseFlag || readFlag || notifyFlag;
    ui->labelUnsupported->setVisible(!supported);

    if (notifyFlag) {
        bool isNotified = mBleTool->isNotified(ch);
        QString txt = isNotified ? tr("Disnotify") : tr("Notify");
        ui->pushButtonNotify->setText(txt);
    }
}

void xToolsBleCentralToolUi::onComboBoxWriteWayCurrentIndexChanged()
{
    int index = ui->comboBoxWriteWay->currentIndex();
    mBleTool->setWriteModel(index);
    qInfo() << "set write model to:" << index;
}

void xToolsBleCentralToolUi::onPushButtonNotifyClicked()
{
    mBleTool->changeNotify();
}

void xToolsBleCentralToolUi::onPushButtonReadClicked()
{
    mBleTool->readCharacteristic();
}
