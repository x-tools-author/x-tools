/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregisterdelegate.h"

#include <QAbstractItemModel>
#include <QCheckbox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#include "modbusregistertable.h"
#include "modbusregistertablefilter.h"

namespace xModbus {

ModbusRegisterDelegate::ModbusRegisterDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

ModbusRegisterDelegate::~ModbusRegisterDelegate() {}

ModbusRegisterTable *getRegisterTable(const QModelIndex &index)
{
    const auto *model = index.model();
    const auto *filter = qobject_cast<const ModbusRegisterTableFilter *>(model);
    if (!filter) {
        return nullptr;
    }

    const auto *sourceModel = filter->sourceModel();
    const auto *registerTable = qobject_cast<const ModbusRegisterTable *>(sourceModel);
    return const_cast<ModbusRegisterTable *>(registerTable);
}

QWidget *ModbusRegisterDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    int column = index.column();
    switch (column) {
    case REGISTER_TABLE_TYPE:
        return new QComboBox(parent);
    case REGISTER_TABLE_SERVER_ADDRESS:
    case REGISTER_TABLE_MIN:
    case REGISTER_TABLE_MAX:
    case REGISTER_TABLE_DECIMALS:
    case REGISTER_TABLE_VALUE:
        return new QSpinBox(parent);
    case REGISTER_TABLE_NAME:
    case REGISTER_TABLE_UNIT:
    case REGISTER_TABLE_DESCRIPTION:
        return new QLineEdit(parent);
    default:
        return nullptr;
    }
}

void ModbusRegisterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!editor || !index.isValid()) {
        return;
    }

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit) {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        lineEdit->setText(value);
    }
}

void ModbusRegisterDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const
{}

} // namespace xModbus
