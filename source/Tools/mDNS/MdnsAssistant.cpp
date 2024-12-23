/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "MdnsAssistant.h"
#include "ui_MdnsAssistant.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLineEdit>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "Common/xTools.h"
#include "MdnsServer.h"

MdnsAssistant::MdnsAssistant(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MdnsAssistant)
{
    qRegisterMetaType<QMdnsEngine::Service>("Service");

    ui->setupUi(this);
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->comboBox->lineEdit()->setClearButtonEnabled(true);

    loadSettings();

    m_server = new MdnsServer(this);
    connect(m_server, &MdnsServer::serviceAdded, this, &MdnsAssistant::onServiceAdded);
    connect(m_server, &MdnsServer::serviceUpdated, this, &MdnsAssistant::onServiceUpdated);
    connect(m_server, &MdnsServer::serviceRemoved, this, &MdnsAssistant::onServiceRemoved);
    connect(m_server, &MdnsServer::started, this, [=]() { setUiState(true); });
    connect(m_server, &MdnsServer::finished, this, [=]() { setUiState(false); });

    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=]() {
        if (m_server->isRunning()) {
            stopMdnsService();
        } else {
            startMdnsService();
        }
    });
}

MdnsAssistant::~MdnsAssistant()
{
    delete ui;
}

void MdnsAssistant::startMdnsService()
{
    QString txt = ui->comboBox->currentText().trimmed();
    if (txt.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Error occurred"),
                             tr("Service type can not be empty, please input then try again."));
        return;
    }

    ui->pushButtonStart->setEnabled(false);
    ui->treeWidget->clear();
    m_server->setServiceType(txt.toUtf8());
    m_server->start();
}

void MdnsAssistant::stopMdnsService()
{
    ui->pushButtonStart->setEnabled(false);
    m_server->exit();
    m_server->wait();
}

void MdnsAssistant::setupItem(const QMdnsEngine::Service &service)
{
    auto item = findItem(service);
    if (item) {
        auto items = item->takeChildren();
        for (auto item : items) {
            delete item;
        }
    } else {
        item = new QTreeWidgetItem(QStringList() << service.name());
        item->setExpanded(true);
        ui->treeWidget->addTopLevelItem(item);
    }

    auto type = QString::fromLatin1(service.type());
    QTreeWidgetItem *typeItem = new QTreeWidgetItem(QStringList() << tr("Type") << type);
    item->addChild(typeItem);

    auto hostname = QString::fromUtf8(service.hostname());
    QTreeWidgetItem *hostnameItem = new QTreeWidgetItem(QStringList() << tr("Hostname") << hostname);
    item->addChild(hostnameItem);

    auto port = QString::number(service.port());
    QTreeWidgetItem *portItem = new QTreeWidgetItem(QStringList() << tr("Port") << port);
    item->addChild(portItem);

    auto attributes = service.attributes();
    QTreeWidgetItem *attrItems = new QTreeWidgetItem(QStringList() << tr("Attributes"));
    item->addChild(attrItems);
    attrItems->setExpanded(true);

    for (auto it = attributes.begin(); it != attributes.end(); ++it) {
        auto key = QString::fromUtf8(it.key());
        auto value = QString::fromUtf8(it.value());
        QTreeWidgetItem *attrItem = new QTreeWidgetItem(QStringList() << key << value);
        attrItems->addChild(attrItem);
    }
}

void MdnsAssistant::setUiState(bool isRunning)
{
    ui->pushButtonStart->setText(isRunning ? tr("Stop") : tr("Start"));
    ui->pushButtonStart->setEnabled(true);
    ui->comboBox->setEnabled(!isRunning);

    if (isRunning && (ui->comboBox->findText(ui->comboBox->currentText()) == -1)) {
        ui->comboBox->addItem(ui->comboBox->currentText());
        saveSettings();
    }
}

void MdnsAssistant::loadSettings()
{
    QSettings *settings = g_xTools.settings();

    auto hexString = settings->value(m_settingKeys.serviceTypes).toString().toLatin1();
    QByteArray json = QByteArray::fromHex(hexString);
    QVariantList list = QJsonDocument::fromJson(json).array().toVariantList();
    for (auto it = list.begin(); it != list.end(); ++it) {
        auto serviceType = it->toString();
        ui->comboBox->addItem(serviceType);
    }

    auto currentServiceType = settings->value(m_settingKeys.currentServiceType).toString();
    if (!currentServiceType.isEmpty()) {
        ui->comboBox->setCurrentText(currentServiceType);
    }

    if (!ui->comboBox->count()) {
        ui->comboBox->addItem("_http._tcp.local.");
    }
}

void MdnsAssistant::saveSettings()
{
    QSettings *settings = g_xTools.settings();

    QVariantList list;
    for (int i = 0; i < ui->comboBox->count(); i++) {
        auto serviceType = ui->comboBox->itemText(i);
        list.append(serviceType);
    }

    QJsonArray array = QJsonArray::fromVariantList(list);
    QJsonDocument doc = QJsonDocument(array);
    QByteArray json = doc.toJson();
    QByteArray hexString = json.toHex();
    settings->setValue(m_settingKeys.serviceTypes, QString::fromLatin1(hexString));

    auto currentServiceType = ui->comboBox->currentText();
    settings->setValue(m_settingKeys.currentServiceType, currentServiceType);
}

QTreeWidgetItem *MdnsAssistant::findItem(const QMdnsEngine::Service &service)
{
    auto name = service.name();
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        auto item = ui->treeWidget->topLevelItem(i);
        if (item->text(0) == name) {
            return item;
        }
    }

    return Q_NULLPTR;
}

void MdnsAssistant::onServiceAdded(const QMdnsEngine::Service &service)
{
    qInfo() << "Service added:" << service;

    setupItem(service);
}

void MdnsAssistant::onServiceUpdated(const QMdnsEngine::Service &service)
{
    qInfo() << "Service updated:" << service;

    setupItem(service);
}

void MdnsAssistant::onServiceRemoved(const QMdnsEngine::Service &service)
{
    qInfo() << "Service removed:" << service;

    auto name = service.name();
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        auto item = ui->treeWidget->topLevelItem(i);
        if (item->text(0) == name) {
            auto item = ui->treeWidget->takeTopLevelItem(i);
            delete item;
            break;
        }
    }
}
