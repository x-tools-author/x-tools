/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialporttransferdelegate.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

#include "common/xtools.h"

SerialPortTransferDelegate::SerialPortTransferDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

SerialPortTransferDelegate::~SerialPortTransferDelegate() {}

QWidget *SerialPortTransferDelegate::createEditor(QWidget *parent,
                                                  const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) const
{
    Q_UNUSED(option);
    switch (index.column()) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return new QComboBox(parent);
    case 7:
        return new QLineEdit(parent);
    default:
        return nullptr;
    }
}

void SerialPortTransferDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int column = index.column();
    if (column >= 0 && column <= 6) {
        QComboBox *cb = nullptr;
        if (column == 0) {
            cb = qobject_cast<QComboBox *>(editor);
            setupTransferType(cb);
        } else if (column == 1) {
            cb = qobject_cast<QComboBox *>(editor);
            setupPortName(cb);
        } else if (column == 2) {
            cb = qobject_cast<QComboBox *>(editor);
            setupBaudRate(cb);
        } else if (column == 3) {
            cb = qobject_cast<QComboBox *>(editor);
            setupDataBits(cb);
        } else if (column == 4) {
            cb = qobject_cast<QComboBox *>(editor);
            setupStopBits(cb);
        } else if (column == 5) {
            cb = qobject_cast<QComboBox *>(editor);
            setupParity(cb);
        } else if (column == 6) {
            cb = qobject_cast<QComboBox *>(editor);
            setupFlowControl(cb);
        }

        QVariant value = index.data(Qt::EditRole);
        if (column == 1) {
            cb->setCurrentIndex(cb->findText(value.toString()));
        } else {
            cb->setCurrentIndex(cb->findData(value));
        }
    } else if (column == 7) {
        qobject_cast<QLineEdit *>(editor)->setText(index.data(Qt::EditRole).toString());
    }
}

void SerialPortTransferDelegate::setModelData(QWidget *editor,
                                              QAbstractItemModel *model,
                                              const QModelIndex &index) const
{
    int column = index.column();
    if (column == 1) {
        auto cb = qobject_cast<QComboBox *>(editor);
        model->setData(index, cb->currentText());
    } else if ((column == 0) || (column > 1 && column < 7)) {
        auto cb = qobject_cast<QComboBox *>(editor);
        model->setData(index, cb->currentData());
    }
}
