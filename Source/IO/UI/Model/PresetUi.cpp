/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "PresetUi.h"

#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>

#include "../../IO/AbstractIO.h"

namespace xTools {

PresetUi::PresetUi(QWidget *parent)
    : AbstractModelUi(parent)
{
    m_menu = new QMenu(this);

    auto *tv = tableView();
    auto hHeader = tv->horizontalHeader();
    hHeader->setStretchLastSection(true);
}

PresetUi::~PresetUi() {}

QMenu *PresetUi::menu()
{
    return m_menu;
}

QVariantMap PresetUi::save() const
{
    QVariantMap map;
    return map;
}

void PresetUi::load(const QVariantMap &parameters)
{
    auto items = parameters.value("items").toJsonArray();
    if (items.isEmpty()) {
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        auto item = items.at(i).toObject();
        m_model->insertColumns(i, 1);
    }
}

void PresetUi::setupIO(AbstractIO *io)
{
    AbstractModelUi::setupIO(io);
}

QList<int> PresetUi::universalColumns() const
{
    return QList<int>{1};
}

} // namespace xTools
