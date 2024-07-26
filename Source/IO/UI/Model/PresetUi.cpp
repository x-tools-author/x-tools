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
#include <QStandardItemModel>
#include <QTableView>

#include "../../IO/AbstractIO.h"

namespace xTools {

PresetUi::PresetUi(QWidget *parent)
    : AbstractModelUi(parent)
{

}

PresetUi::~PresetUi() {}

QMenu *PresetUi::menu()
{
    return nullptr;
}

QVariantMap PresetUi::save() const
{
    return QVariantMap();
}

void PresetUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void PresetUi::setupIO(AbstractIO *io)
{
    Q_UNUSED(io);
}

void PresetUi::onBaseToolUiInitialized(AbstractIO *tool, const QString &settingGroup)
{
    Q_UNUSED(tool);
    Q_UNUSED(settingGroup);
}

QList<int> PresetUi::defaultHideColumns()
{
    QList<int> list;

    return list;
}

void PresetUi::afterRowEdited(int row) {}

QDialog *PresetUi::itemEditor()
{
    return nullptr;
}

} // namespace xTools
