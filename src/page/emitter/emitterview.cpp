/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "emitterview.h"

#include <QCheckBox>
#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTimer>

#include "emittermodel.h"

EmitterView::EmitterView(QWidget *parent)
    : TableView(parent)
{
    m_tableModel = new EmitterModel(this);
    setTableModel(m_tableModel);

    QTableView *tv = tableView();
    QHeaderView *hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);

    QTimer *timer = new QTimer(this);
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() {
        try2Output();
        timer->start();
    });
    timer->start();
}

EmitterView::~EmitterView() {}

QList<int> EmitterView::textItemColumns() const
{
    return QList<int>{3};
}

void EmitterView::try2Output()
{
    int rows = m_tableModel->rowCount(QModelIndex());
    for (int i = 0; i < rows; ++i) {
        m_tableModel->increaseElapsedTime(i, 10);
        if (!m_tableModel->isTimeout(i)) {
            continue;
        }

        m_tableModel->resetElapsedTime(i);

        if (!m_tableModel->data(m_tableModel->index(i, 0), Qt::EditRole).toBool()) {
            continue;
        }

        QVariant var = m_tableModel->data(m_tableModel->index(i, 3), Qt::EditRole);
        QJsonObject json = var.toJsonObject();
        TextItem textItem = loadTextItem(json);
        QByteArray bytes = textItem2array(textItem);

        if (!isDisableAll()) {
            emit outputBytes(bytes);
        }
    }
}
