/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "dataviewui.h"
#include "ui_dataviewui.h"

#include <QDateTime>
#include <QHeaderView>
#include <QStandardItemModel>

#include "common/xtools.h"
#include "dataview.h"
#include "utilities/crc.h"

DataViewUi::DataViewUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::DataViewUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);
#if 0
    setMinimumWidth(520);
#endif
    xSetupTextFormat(ui->comboBoxFormat);

    QStandardItemModel *model = new QStandardItemModel(0, 2, this);
    model->setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Data"));
    ui->tableView->setModel(model);
    ui->tableView->setAlternatingRowColors(true);
    int tmp = int(Qt::AlignLeft | Qt::AlignVCenter);
    model->setHeaderData(0, Qt::Horizontal, tmp, Qt::TextAlignmentRole);
    model->setHeaderData(1, Qt::Horizontal, tmp, Qt::TextAlignmentRole);

    QHeaderView *vHeader = ui->tableView->verticalHeader();
    vHeader->hide();
    QHeaderView *hHeader = ui->tableView->horizontalHeader();
    hHeader->setStretchLastSection(true);

    connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
        int rowCount = model->rowCount();
        model->removeRows(0, rowCount);
    });
    connect(ui->comboBoxFormat, &QComboBox::currentIndexChanged, this, [=]() {
        for (int i = 0; i < model->rowCount(); ++i) {
            QByteArray data = model->data(model->index(i, 1), Qt::UserRole + 1).toByteArray();
            int format = ui->comboBoxFormat->currentData().toInt();
            QString txt = xBytes2string(data, format);
            model->setData(model->index(i, 1), txt, Qt::DisplayRole);
        }
        ui->tableView->update();
    });
    connect(m_node, &BaseNode::dataUpdated, this, &DataViewUi::onDataUpdated);
}

DataViewUi::~DataViewUi()
{
    delete ui;
}

QJsonObject DataViewUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    parameters.insert(DataKey().format, ui->comboBoxFormat->currentData().toInt());
    return parameters;
}

void DataViewUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    QVariantMap parametersMap = parameters.toVariantMap();
    int format = parametersMap.value(DataKey().format, static_cast<int>(TextFormat::Ascii)).toInt();
    int index = ui->comboBoxFormat->findData(format);
    ui->comboBoxFormat->setCurrentIndex(index < 0 ? 0 : index);
}

void DataViewUi::onDataUpdated(const QtNodes::PortIndex portIndex)
{
    auto data = m_node->outData(portIndex);
    auto cookedData = std::dynamic_pointer_cast<BaseNodeData>(data);
    if (!cookedData) {
        return;
    }

    QByteArray bytes = cookedData->bytes();
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());
    model->insertRow(0);
    model->setData(model->index(0, 0), time);
    model->setData(model->index(0, 1), bytes, Qt::UserRole + 1);

    int format = ui->comboBoxFormat->currentData().toInt();
    QString txt = xBytes2string(bytes, format);
    model->setData(model->index(0, 1), txt, Qt::DisplayRole);

    static const int maxRow = 1024;
    while (model->rowCount() > maxRow) {
        model->removeRow(model->rowCount());
    }
}
