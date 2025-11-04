/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregisterdelegate.h"

#include <QCheckbox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

namespace xModbus {

ModbusRegisterDelegate::ModbusRegisterDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

ModbusRegisterDelegate::~ModbusRegisterDelegate() {}

QWidget *ModbusRegisterDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    //return QStyledItemDelegate::createEditor(parent, option, index);
    return nullptr;
}

void ModbusRegisterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //QStyledItemDelegate::setEditorData(editor, index);
}

void ModbusRegisterDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    //QStyledItemDelegate::setModelData(editor, model, index);
}

} // namespace xModbus