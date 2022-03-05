/*
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QFile>
#include <QDebug>
#include <QLineEdit>
#include <QJsonArray>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>

#include "SAKUdpClientAdvanced.hh"
#include "ui_SAKUdpClientAdvanced.h"

#define TABLE_MAX_COLUMNS 10

SAKUdpClientAdvanced::SAKUdpClientAdvanced(QSettings *settings,
                                           const QString settingsGroup,
                                           QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKUdpClientAdvanced)
    ,mSettings(settings)
    ,mGroup(settingsGroup)
{
    ui->setupUi(this);
    setModal(true);
    resize(1024, 600);

    // Initialize table widget.
    QStringList headerLabels;
    headerLabels << tr("Unicast Address")
                 << tr("Multicast Address")
                 << tr("Boardcast Port");
    QTableWidget *tw = ui->tableWidget;
    tw->setColumnCount(headerLabels.length());
    tw->setHorizontalHeaderLabels(headerLabels);
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tw->setRowCount(TABLE_MAX_COLUMNS);
    for (int i = 0; i < headerLabels.length()*tw->rowCount(); i++) {
        int row = i/headerLabels.length();
        int column = i%headerLabels.length();

        QWidget *w = new QWidget(this);
        QCheckBox *cb = new QCheckBox(tr("Enable"), this);
        cb->setObjectName("enable");
        QLineEdit *le = new QLineEdit(this);
        le->setObjectName("textValue");
        if (column == 0) {
            le->setPlaceholderText(QString("192.168.0.168:54545"));
        } else if (column == 1) {
            le->setPlaceholderText(QString("239.0.0.168:54545"));
        } else if (column == 2) {
            le->setPlaceholderText(QString("54545"));
        }
        le->setStyleSheet(QString("QLineEdit{border:0px solid red;"
                                  "background-color:rgba(0,0,0,0)}"));
        le->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Preferred));
        QHBoxLayout *hbl = new QHBoxLayout(w);
        hbl->setContentsMargins(10, 0, 10, 0);
        hbl->addWidget(cb);
        hbl->addWidget(le);
        tw->setCellWidget(row, column, w);
    }

    connect(ui->importPushButton, &QPushButton::clicked,
            this, &SAKUdpClientAdvanced::importParameters);
    connect(ui->exportPushButton, &QPushButton::clicked,
            this, &SAKUdpClientAdvanced::exportParameters);
    connect(ui->clearUnicastPushButton, &QPushButton::clicked,
            this, &SAKUdpClientAdvanced::clearUnicast);
    connect(ui->clearMulticastPushButton, &QPushButton::clicked,
            this, &SAKUdpClientAdvanced::clearMulticast);
    connect(ui->clearBroadcastPushButton, &QPushButton::clicked,
            this, &SAKUdpClientAdvanced::clearBroadCast);
    connect(ui->applyPushButton, &QPushButton::clicked,
            this, &SAKUdpClientAdvanced::updateParameters);

    QJsonObject parameters = readParameterFromIni();
    setParameters(parameters);
}

SAKUdpClientAdvanced::~SAKUdpClientAdvanced()
{
    delete ui;
}

void SAKUdpClientAdvanced::updateParameters()
{
    QJsonObject parameters = this->parameters();
    writeParametersToini(parameters);
    emit parametersUpdated(parameters);
}

const QJsonObject SAKUdpClientAdvanced::parameters()
{
    QJsonObject jsonObj;
    QTableWidget *tw = ui->tableWidget;
    jsonObj.insert("enable", ui->enableAdvancedCheckBox->isChecked());
    jsonObj.insert("enableUnicast", ui->unicastCheckBox->isChecked());
    jsonObj.insert("enableMulticast", ui->multicastCheckBox->isChecked());
    jsonObj.insert("enableBoradcast", ui->broadcastCheckBox->isChecked());

    QJsonArray unicastArray;
    QJsonArray multicastArray;
    QJsonArray broadcastArray;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        // Extract unicast info.
        QJsonObject unicastCtx;
        QWidget *unicastW = tw->cellWidget(i, 0);
        QCheckBox *unicastCB =
                (QCheckBox*)(unicastW->layout()->itemAt(0)->widget());
        QLineEdit *unicastLE =
                (QLineEdit*)(unicastW->layout()->itemAt(1)->widget());
        unicastCtx.insert("enable", unicastCB->isChecked());
        unicastCtx.insert("value", unicastLE->text().trimmed());
        unicastArray.append(unicastCtx);

        // Extract multicast info.
        QJsonObject multicastCtx;
        QWidget *multicastW = tw->cellWidget(i, 1);
        QCheckBox *multicastCB =
                (QCheckBox*)(multicastW->layout()->itemAt(0)->widget());
        QLineEdit *multicastLE =
                (QLineEdit*)(multicastW->layout()->itemAt(1)->widget());
        multicastCtx.insert("enable", multicastCB->isChecked());
        multicastCtx.insert("value", multicastLE->text().trimmed());
        multicastArray.append(multicastCtx);

        // Extract broadcast info.
        QJsonObject broadcastCtx;
        QWidget *broadcastW = tw->cellWidget(i, 2);
        QCheckBox *broadcastCB =
                (QCheckBox*)(broadcastW->layout()->itemAt(0)->widget());
        QLineEdit *broadcastLE =
                (QLineEdit*)(broadcastW->layout()->itemAt(1)->widget());
        broadcastCtx.insert("enable", broadcastCB->isChecked());
        broadcastCtx.insert("value", broadcastLE->text().trimmed());
        broadcastArray.append(broadcastCtx);
    }

    jsonObj.insert("unicastCtx", unicastArray);
    jsonObj.insert("multicastCtx", multicastArray);
    jsonObj.insert("broadcastCtx", broadcastArray);
    return jsonObj;
}

void SAKUdpClientAdvanced::setParameters(const QJsonObject &parameters)
{
    bool enable = parameters.value("enable").toBool();
    bool enableUnicast = parameters.value("enableUnicast").toBool();
    bool enableMulticast = parameters.value("enableMulticast").toBool();
    bool enableBoradcast = parameters.value("enableBoradcast").toBool();
    ui->enableAdvancedCheckBox->setChecked(enable);
    ui->unicastCheckBox->setChecked(enableUnicast);
    ui->multicastCheckBox->setChecked(enableMulticast);
    ui->broadcastCheckBox->setChecked(enableBoradcast);

    QJsonArray unicastArray = parameters.value("unicastCtx").toArray();
    QJsonArray multicastArray = parameters.value("multicastCtx").toArray();
    QJsonArray broadcastArray = parameters.value("broadcastCtx").toArray();

    QTableWidget *tw = ui->tableWidget;
    for (int i = 0; i < qMin<int>(tw->rowCount(), unicastArray.count()); i++) {
        QWidget *unicastW = tw->cellWidget(i, 0);
        QCheckBox *unicastCB =
                (QCheckBox*)(unicastW->layout()->itemAt(0)->widget());
        QLineEdit *unicastLE =
                (QLineEdit*)(unicastW->layout()->itemAt(1)->widget());

        QJsonObject ctx = unicastArray.at(i).toObject();
        unicastCB->setChecked(ctx.value("enable").toBool());
        unicastLE->setText(ctx.value("value").toString());
    }

    for (int i = 0; i < qMin<int>(tw->rowCount(), multicastArray.count()); i++) {
        QWidget *multicastW = tw->cellWidget(i, 1);
        QCheckBox *multicastCB =
                (QCheckBox*)(multicastW->layout()->itemAt(0)->widget());
        QLineEdit *multicastLE =
                (QLineEdit*)(multicastW->layout()->itemAt(1)->widget());

        QJsonObject ctx = multicastArray.at(i).toObject();
        multicastCB->setChecked(ctx.value("enable").toBool());
        multicastLE->setText(ctx.value("value").toString());
    }

    for (int i = 0; i < qMin<int>(tw->rowCount(), broadcastArray.count()); i++) {
        QWidget *broadcastW = tw->cellWidget(i, 2);
        QCheckBox *broadcastCB =
                (QCheckBox*)(broadcastW->layout()->itemAt(0)->widget());
        QLineEdit *broadcastLE =
                (QLineEdit*)(broadcastW->layout()->itemAt(1)->widget());

        QJsonObject ctx = broadcastArray.at(i).toObject();
        broadcastCB->setChecked(ctx.value("enable").toBool());
        broadcastLE->setText(ctx.value("value").toString());
    }
}

void SAKUdpClientAdvanced::exportParameters()
{
    QString path =
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    path += "./UDPClientParameters.json";
    const QString fileName = QFileDialog::getSaveFileName(
                this, tr("Export Parameters"),  path, QString("json (*.json)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly|QFile::Text)) {
            QTextStream out(&file);
            QJsonDocument jsonDoc;
            jsonDoc.setObject(parameters());
            out << jsonDoc.toJson();
            file.close();
        } else {
            qWarning() << file.errorString();
        }
    }
}

void SAKUdpClientAdvanced::importParameters()
{
    QString path =
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    path += "./UDPClientParameters.json";
    const QString fileName = QFileDialog::getOpenFileName(
                this, tr("Import Parameters"),  path, QString("json (*.json)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly|QFile::Text)) {
            QByteArray json = file.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
            setParameters(jsonDoc.object());
            file.close();
        } else {
            qWarning() << file.errorString();
        }
    }
}

void SAKUdpClientAdvanced::clearUnicast()
{
    if (showClearMessageBox() == QMessageBox::Ok) {
        QTableWidget *tw = ui->tableWidget;
        for (int i = 0; i < tw->rowCount(); i++) {
            QWidget *unicastW = tw->cellWidget(i, 0);
            QCheckBox *unicastCB =
                    (QCheckBox*)(unicastW->layout()->itemAt(0)->widget());
            QLineEdit *unicastLE =
                    (QLineEdit*)(unicastW->layout()->itemAt(1)->widget());
            Q_UNUSED(unicastCB);
            unicastLE->clear();
        }
    }
}

void SAKUdpClientAdvanced::clearMulticast()
{
    if (showClearMessageBox() == QMessageBox::Ok) {
        QTableWidget *tw = ui->tableWidget;
        for (int i = 0; i < tw->rowCount(); i++) {
            QWidget *multicastW = tw->cellWidget(i, 1);
            QCheckBox *multicastCB =
                    (QCheckBox*)(multicastW->layout()->itemAt(0)->widget());
            QLineEdit *multicastLE =
                    (QLineEdit*)(multicastW-> layout()->itemAt(1)->widget());
            Q_UNUSED(multicastCB);
            multicastLE->clear();
        }
    }
}

void SAKUdpClientAdvanced::clearBroadCast()
{
    if (showClearMessageBox() == QMessageBox::Ok) {
        QTableWidget *tw = ui->tableWidget;
        for (int i = 0; i < tw->rowCount(); i++) {
            QWidget *broadcastW = tw->cellWidget(i, 2);
            QCheckBox *broadcastCB =
                    (QCheckBox*)(broadcastW->layout()->itemAt(0)->widget());
            QLineEdit *broadcastLE =
                    (QLineEdit*)(broadcastW->layout()->itemAt(1)->widget());
            Q_UNUSED(broadcastCB);
            broadcastLE->clear();
        }
    }
}

void SAKUdpClientAdvanced::writeParametersToini(const QJsonObject &parameters)
{
    bool enable = parameters.value("enable").toBool();
    bool enableUnicast = parameters.value("enableUnicast").toBool();
    bool enableMulticast = parameters.value("enableMulticast").toBool();
    bool enableBoradcast = parameters.value("enableBoradcast").toBool();

    // Write data to ini file.
    mSettings->beginGroup(mGroup);
    mSettings->setValue("enableAdvanced", enable);
    mSettings->setValue("enableUnicast", enableUnicast);
    mSettings->setValue("enableMulticast", enableMulticast);
    mSettings->setValue("enableBoradcast", enableBoradcast);

    QJsonArray unicastArray = parameters.value("unicastCtx").toArray();
    QJsonArray multicastArray = parameters.value("multicastCtx").toArray();
    QJsonArray broadcastArray = parameters.value("broadcastCtx").toArray();

    mSettings->beginWriteArray("unicastCtx");
    for (int i = 0; i < unicastArray.count(); i++) {
        mSettings->setArrayIndex(i);
        QJsonObject jsonObj = unicastArray.at(i).toObject();
        mSettings->setValue("enable", jsonObj.value("enable").toBool());
        mSettings->setValue("value", jsonObj.value("value").toString());
    }
    mSettings->endArray();

    mSettings->beginWriteArray("multicastCtx");
    for (int i = 0; i < multicastArray.count(); i++) {
        mSettings->setArrayIndex(i);
        QJsonObject jsonObj = multicastArray.at(i).toObject();
        mSettings->setValue("enable", jsonObj.value("enable").toBool());
        mSettings->setValue("value", jsonObj.value("value").toString());
    }
    mSettings->endArray();

    mSettings->beginWriteArray("broadcastCtx");
    for (int i = 0; i < broadcastArray.count(); i++) {
        mSettings->setArrayIndex(i);
        QJsonObject jsonObj = broadcastArray.at(i).toObject();
        mSettings->setValue("enable", jsonObj.value("enable").toBool());
        mSettings->setValue("value", jsonObj.value("value").toString());
    }
    mSettings->endArray();

    mSettings->endGroup();
}

QJsonObject SAKUdpClientAdvanced::readParameterFromIni()
{
    mSettings->beginGroup(mGroup);
    bool enable = mSettings->value("enableAdvanced").toBool();
    bool enableUnicast = mSettings->value("enableUnicast").toBool();
    bool enableMulticast = mSettings->value("enableMulticast").toBool();
    bool enableBoradcast = mSettings->value("enableBoradcast").toBool();

    QJsonObject jsonObj;
    jsonObj.insert("enable", enable);
    jsonObj.insert("enableUnicast", enableUnicast);
    jsonObj.insert("enableMulticast", enableMulticast);
    jsonObj.insert("enableBoradcast", enableBoradcast);

    QJsonArray unicastArray;
    QJsonArray multicastArray;
    QJsonArray broadcastArray;

    mSettings->beginWriteArray("unicastCtx");
    for (int i = 0; i < TABLE_MAX_COLUMNS; i++) {
        mSettings->setArrayIndex(i);
        bool enable = mSettings->value("enable").toBool();
        QString value = mSettings->value("value").toString();
        QJsonObject jsonObj;
        jsonObj.insert("enable", enable);
        jsonObj.insert("value", value);
        unicastArray.append(jsonObj);
    }
    mSettings->endArray();

    mSettings->beginWriteArray("multicastCtx");
    for (int i = 0; i < TABLE_MAX_COLUMNS; i++) {
        mSettings->setArrayIndex(i);
        bool enable = mSettings->value("enable").toBool();
        QString value = mSettings->value("value").toString();
        QJsonObject jsonObj;
        jsonObj.insert("enable", enable);
        jsonObj.insert("value", value);
        multicastArray.append(jsonObj);
    }
    mSettings->endArray();

    mSettings->beginWriteArray("broadcastCtx");
    for (int i = 0; i < TABLE_MAX_COLUMNS; i++) {
        mSettings->setArrayIndex(i);
        bool enable = mSettings->value("enable").toBool();
        QString value = mSettings->value("value").toString();
        QJsonObject jsonObj;
        jsonObj.insert("enable", enable);
        jsonObj.insert("value", value);
        broadcastArray.append(jsonObj);
    }
    mSettings->endArray();

    mSettings->endGroup();

    jsonObj.insert("unicastCtx", unicastArray);
    jsonObj.insert("multicastCtx", multicastArray);
    jsonObj.insert("broadcastCtx", broadcastArray);
    return jsonObj;
}

int SAKUdpClientAdvanced::showClearMessageBox()
{
    int ret = QMessageBox::warning(this,
                                   tr("Clear Data"),
                                   tr("All data inputed will be clear, "
                                      "ensure to do this?"),
                                   QMessageBox::Ok|QMessageBox::Cancel);
    return ret;
}
