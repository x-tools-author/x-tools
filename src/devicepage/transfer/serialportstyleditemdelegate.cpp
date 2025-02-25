/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialportstyleditemdelegate.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

#include "IO/xIO.h"

namespace xTools {

SerialPortStyledItemDelegate::SerialPortStyledItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

SerialPortStyledItemDelegate::~SerialPortStyledItemDelegate() {}

QWidget *SerialPortStyledItemDelegate::createEditor(QWidget *parent,
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

void SerialPortStyledItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int column = index.column();
    if (column >= 0 && column <= 6) {
        QComboBox *cb = nullptr;
        if (column == 0) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupTransferType(cb);
        } else if (column == 1) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupPortName(cb);
        } else if (column == 2) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupBaudRate(cb);
        } else if (column == 3) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupDataBits(cb);
        } else if (column == 4) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupStopBits(cb);
        } else if (column == 5) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupParity(cb);
        } else if (column == 6) {
            cb = qobject_cast<QComboBox *>(editor);
            xIO::setupFlowControl(cb);
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

void SerialPortStyledItemDelegate::setModelData(QWidget *editor,
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

} // namespace xTools
