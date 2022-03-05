/*
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKUdpServerMulticast.hh"
#include "ui_SAKUdpServerMulticast.h"

#define TABLE_MAX_COLUMNS 10

SAKUdpServerMulticast::SAKUdpServerMulticast(QSettings *settings,
                                             const QString settingsGroup,
                                             QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKUdpServerMulticast)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
{
    ui->setupUi(this);
    ui->tipsLabel->setText(" ");
    resize(1024, 500);
    setModal(true);

    QStringList headerLabels;
    headerLabels << tr("Options")
                 << tr("Multicast Address");
    QTableWidget *tw = ui->tableWidget;
    tw->setColumnCount(headerLabels.length());
    tw->setHorizontalHeaderLabels(headerLabels);
    tw->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tw->setRowCount(TABLE_MAX_COLUMNS);
    const QString tips = tr("Parameters have been changed! "
                            "Click the \"Apply\" please!");
    for (int i = 0; i < headerLabels.length()*tw->rowCount(); i++) {
        int row = i/headerLabels.length();
        int column = i%headerLabels.length();

        QWidget *w;
        if (column == 0) {
            QCheckBox *cb = new QCheckBox(tr("Join"), this);
            connect(cb, &QCheckBox::clicked, this, [=](){
                ui->tipsLabel->setText(tips);
            });
            w = cb;
        } else {
            QLineEdit *le = new QLineEdit(this);
            connect(le, &QLineEdit::textChanged, this, [=](){
                ui->tipsLabel->setText(tips);
            });
            le->setPlaceholderText(QString("239.0.0.168"));
            le->setStyleSheet(QString("QLineEdit{border:0px solid red;"
                                      "background-color:rgba(0,0,0,0)}"));
            le->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Preferred));
            w = le;
        }

        QWidget *widget = new QWidget(this);
        QHBoxLayout *l = new QHBoxLayout(widget);
        l->addWidget(w);
        l->setContentsMargins(10, 0, 10, 0);
        tw->setCellWidget(row, column, widget);
    }

    connect(ui->applyPushButton, &QPushButton::clicked, this, [=](){
        ui->tipsLabel->setText(" ");
        updateParameters();
    });

    setParameters(readParameterFromIni());
    ui->tipsLabel->setText(" ");
}

SAKUdpServerMulticast::~SAKUdpServerMulticast()
{
    delete ui;
}

const QJsonArray SAKUdpServerMulticast::parameters()
{
    QTableWidget *tw = ui->tableWidget;
    QJsonArray paras;
    for (int i = 0; i < tw->rowCount(); i++) {
        QCheckBox *cb = cellCheckBox(i);
        QLineEdit *le = cellLineEdit(i);

        QJsonObject jsonObj;
        jsonObj.insert("enable", cb->isChecked());
        jsonObj.insert("value", le->text().trimmed());
        paras.append(jsonObj);
    }

    return paras;
}

void SAKUdpServerMulticast::updateUiState(bool opened)
{
    ui->applyPushButton->setEnabled(!opened);
    ui->tableWidget->setEnabled(!opened);
}

void SAKUdpServerMulticast::setParameters(const QJsonArray &parameters)
{
    int size = qMin(parameters.count(), ui->tableWidget->rowCount());
    for (int i = 0; i < size; i++) {
        QJsonObject jsonObj = parameters.at(i).toObject();
        bool enable = jsonObj.value("enable").toBool();
        QString value = jsonObj.value("value").toString();

        QCheckBox *cb = cellCheckBox(i);
        QLineEdit *le = cellLineEdit(i);
        cb->setChecked(enable);
        le->setText(value);
    }
}

void SAKUdpServerMulticast::updateParameters()
{
    QJsonArray parameters = this->parameters();
    writeParametersToini(parameters);
    emit parametersChanged(parameters);
}

void SAKUdpServerMulticast::writeParametersToini(const QJsonArray &parameters)
{
    mSettings->beginGroup(mSettingsGroup);
    mSettings->beginReadArray("MulticastAddress");
    for (int i = 0; i < TABLE_MAX_COLUMNS; i++) {
        mSettings->setArrayIndex(i);
        QJsonObject jsonObj = parameters.at(i).toObject();
        bool enable = jsonObj.value("enable").toBool();
        QString value = jsonObj.value("value").toString();

        mSettings->setValue("enable", enable);
        mSettings->setValue("value", value);
    }
    mSettings->endArray();
    mSettings->endGroup();
}

const QJsonArray SAKUdpServerMulticast::readParameterFromIni()
{
    QJsonArray parameters;
    mSettings->beginGroup(mSettingsGroup);
    mSettings->beginReadArray("MulticastAddress");
    for (int i = 0; i < TABLE_MAX_COLUMNS; i++) {
        mSettings->setArrayIndex(i);
        bool enable = mSettings->value("enable").toBool();
        QString value = mSettings->value("value").toString();

        QJsonObject jsonObj;
        jsonObj.insert("enable", enable);
        jsonObj.insert("value", value);
        parameters.append(jsonObj);
    }
    mSettings->endArray();
    mSettings->endGroup();

    return parameters;
}

QCheckBox *SAKUdpServerMulticast::cellCheckBox(int row)
{
    QWidget *w = cellWidget(row, 0);
    QCheckBox *cb = (QCheckBox*)(w);
    return cb;
}

QLineEdit *SAKUdpServerMulticast::cellLineEdit(int row)
{
    QWidget *w = cellWidget(row, 1);
    QLineEdit *le = (QLineEdit*)(w);
    return le;
}

QWidget *SAKUdpServerMulticast::cellWidget(int row, int column)
{
    QTableWidget *tw = ui->tableWidget;
    QWidget *cellWidget = tw->cellWidget(row, column);
    QLayout *cellWidgetLayout = cellWidget->layout();
    QWidget *w = cellWidgetLayout->itemAt(0)->widget();
    return w;
}
