/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketStyledItemDelegate.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

#include "../../xIO.h"

namespace xTools {

SocketStyledItemDelegate::SocketStyledItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

SocketStyledItemDelegate::~SocketStyledItemDelegate() {}

QWidget *SocketStyledItemDelegate::createEditor(QWidget *parent,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    switch (index.column()) {
    case 6: // authentication
        return new QCheckBox(parent);
    case 0: // option
    case 1: // client address
    case 3: // server address
    case 5: // data channel
        return new QComboBox(parent);
    case 2: // client port
    case 4: // server port
        return new QSpinBox(parent);
    case 7: // username
    case 8: // password
    case 9: // description
        return new QLineEdit(parent);
    default:
        return nullptr;
    }
}

void SocketStyledItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int column = index.column();
    if (column == 6) {
        qobject_cast<QCheckBox *>(editor)->setChecked(index.data(Qt::EditRole).toBool());
    } else if (column == 0 || column == 1 || column == 3 || column == 5) {
        auto cb = qobject_cast<QComboBox *>(editor);
        if (column == 0) {
            xIO::setupTransferType(cb);
        } else if (column == 1 || column == 3) {
            xIO::setupSocketAddress(cb);
        } else if (column == 5) {
            xIO::setupWebSocketDataChannel(cb);
        }

        QVariant value = index.data(Qt::EditRole);
        cb->setCurrentIndex(cb->findData(value));
    } else if (column == 2 || column == 4) {
        auto sb = qobject_cast<QSpinBox *>(editor);
        xIO::setupSocketPort(sb);
        sb->setValue(index.data(Qt::EditRole).toInt());
    } else if (column == 8 || column == 9 || column == 10) {
        auto le = qobject_cast<QLineEdit *>(editor);
        le->setText(index.data(Qt::EditRole).toString());
    }
}

void SocketStyledItemDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    int column = index.column();
    if (column == 6) {
        model->setData(index, qobject_cast<QCheckBox *>(editor)->isChecked(), Qt::EditRole);
    } else if (column == 0 || column == 1 || column == 3 || column == 5) {
        auto cb = qobject_cast<QComboBox *>(editor);
        model->setData(index, cb->currentData(), Qt::EditRole);
    } else if (column == 2 || column == 4) {
        auto sb = qobject_cast<QSpinBox *>(editor);
        model->setData(index, sb->value(), Qt::EditRole);
    } else if (column == 8 || column == 9 || column == 10) {
        auto le = qobject_cast<QLineEdit *>(editor);
        model->setData(index, le->text(), Qt::EditRole);
    }
}

} // namespace xTools
