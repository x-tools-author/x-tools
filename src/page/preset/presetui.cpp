/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "presetui.h"

#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>

#include "common/xtools.h"
#include "device/utilities/menu.h"
#include "page/common/abstractmodelio.h"

PresetUi::PresetUi(QWidget *parent)
    : AbstractModelUi(parent)
{
    m_menu = new Menu();
    setDisableCheckBoxVisible(false);

    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

PresetUi::~PresetUi() {}

QMenu *PresetUi::menu()
{
    return m_menu;
}

void PresetUi::setupIO(AbstractIO *io)
{
    AbstractModelUi::setupIO(io);
    connect(m_model, &QAbstractTableModel::dataChanged, this, &PresetUi::onDataChanged);
    connect(m_model, &QAbstractTableModel::rowsRemoved, this, &PresetUi::onDataChanged);
    connect(m_model, &QAbstractTableModel::rowsInserted, this, &PresetUi::onDataChanged);
}

QList<int> PresetUi::textItemColumns() const
{
    return QList<int>{1};
}

void PresetUi::didOutputBytes(int row)
{
    auto rows = m_model->rowCount();
    if (row < 0 || row >= rows) {
        return;
    }

    QModelIndex index = m_model->index(row, 1);
    QJsonObject rawItem = m_model->data(index, Qt::EditRole).toJsonObject();
    TextItem textItem = loadTextItem(rawItem);
    QByteArray bytes = textItem2array(textItem);
    emit m_io->outputBytes(bytes);
}

void PresetUi::onDataChanged()
{
    m_menu->clear();
    int rows = m_model->rowCount();

    for (int i = 0; i < rows; ++i) {
        auto index = m_model->index(i, 0);
        auto text = m_model->data(index, Qt::DisplayRole).toString();
        m_menu->addAction(text, this, [=]() { didOutputBytes(i); });
    }
}
