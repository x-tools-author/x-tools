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

#include "modbuscommon.h"
#include "modbusregistertable.h"
#include "modbusregistertablefilter.h"

namespace xModbus {

ModbusRegisterDelegate::ModbusRegisterDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

ModbusRegisterDelegate::~ModbusRegisterDelegate() {}

ModbusRegisterTableFilter *getRegisterTableFilter(const QModelIndex &index)
{
    const auto *model = index.model();
    const auto *filter = qobject_cast<const ModbusRegisterTableFilter *>(model);
    return const_cast<ModbusRegisterTableFilter *>(filter);
}

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

    if (!index.isValid()) {
        return;
    }

    int column = index.column();
    QVariant value = index.model()->data(index, Qt::EditRole);
    if (column == REGISTER_TABLE_TYPE) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        if (comboBox) {
            xModbus::setupRegisterType(comboBox);
            int index = comboBox->findData(value.toInt());
            comboBox->setCurrentIndex(index == -1 ? 0 : index);
        }
    } else if (column == REGISTER_TABLE_SERVER_ADDRESS) {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        if (spinBox) {
            spinBox->setMinimum(0);
            spinBox->setMaximum(255);
            spinBox->setValue(value.toInt());
        }
    } else if (column == REGISTER_TABLE_MIN || column == REGISTER_TABLE_MAX) {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        if (spinBox) {
            spinBox->setMinimum(0);
            spinBox->setMaximum(65535);
            spinBox->setValue(value.toInt());
        }
    } else if (column == REGISTER_TABLE_DECIMALS) {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        if (spinBox) {
            spinBox->setMinimum(0);
            spinBox->setMaximum(5);
            spinBox->setValue(value.toInt());
        }
    } else if (column == REGISTER_TABLE_VALUE) {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        if (spinBox) {
            spinBox->setMinimum(0);
            spinBox->setMaximum(65535);
            spinBox->setValue(value.toInt());
        }
    } else if (column == REGISTER_TABLE_NAME || column == REGISTER_TABLE_UNIT
               || column == REGISTER_TABLE_DESCRIPTION) {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        if (lineEdit) {
            lineEdit->setText(value.toString());
        }
    }
}

void ModbusRegisterDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    if (!editor || !index.isValid()) {
        return;
    }

    int column = index.column();
    auto registerTableFilter = getRegisterTableFilter(index);
    if (!registerTableFilter && !model) {
        return;
    }

    const QModelIndex tableIndex = registerTableFilter->mapToSource(index);
    int row = tableIndex.row();
    auto registerTable = getRegisterTable(index);
    if (!registerTable) {
        return;
    }

    bool isComboBox = column == REGISTER_TABLE_TYPE;
    bool isSpinBox = column == REGISTER_TABLE_SERVER_ADDRESS;
    isSpinBox |= column == REGISTER_TABLE_MIN;
    isSpinBox |= column == REGISTER_TABLE_MAX;
    isSpinBox |= column == REGISTER_TABLE_VALUE;
    isSpinBox |= column == REGISTER_TABLE_DECIMALS;
    bool isLineEdit = column == REGISTER_TABLE_NAME;
    isLineEdit |= column == REGISTER_TABLE_UNIT;
    isLineEdit |= column == REGISTER_TABLE_DESCRIPTION;

    if (isComboBox) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        if (comboBox) {
            int type = comboBox->currentData().toInt();
            registerTable->setData(tableIndex, type, Qt::EditRole);
        }
    } else if (isSpinBox) {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
        if (!spinBox) {
            return;
        }

        int intValue = spinBox->value();
        if (column == REGISTER_TABLE_VALUE) {
            // Clamp the value between min and max
            QModelIndex minIndex = registerTable->index(row, REGISTER_TABLE_MIN);
            QModelIndex maxIndex = registerTable->index(row, REGISTER_TABLE_MAX);
            QVariant minVar = registerTable->data(minIndex, Qt::EditRole);
            QVariant maxVar = registerTable->data(maxIndex, Qt::EditRole);

            int minValue = minVar.toInt();
            int maxValue = maxVar.toInt();
            if (intValue < minValue) {
                intValue = minValue;
            } else if (intValue > maxValue) {
                intValue = maxValue;
            }

            QModelIndex registerTypeIndex = registerTable->index(row, REGISTER_TABLE_TYPE);
            int registerType = registerTable->data(registerTypeIndex, Qt::EditRole).toInt();
            bool isBit = registerType == QModbusDataUnit::Coils;
            isBit |= registerType == QModbusDataUnit::DiscreteInputs;
            if (isBit) {
                intValue = intValue ? 1 : 0;
            }
        }

        registerTable->setData(tableIndex, intValue, Qt::EditRole);
    } else if (isLineEdit) {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        if (lineEdit) {
            QString text = lineEdit->text();
            registerTable->setData(tableIndex, text, Qt::EditRole);
        }
    }
}

} // namespace xModbus
