/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sockettransferdelegate.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>

#include "common/xtools.h"
#include "sockettransfermodel.h"

SocketTransferDelegate::SocketTransferDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

SocketTransferDelegate::~SocketTransferDelegate() {}

QWidget *SocketTransferDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *tmp = nullptr;
    switch (index.column()) {
    case SOCKET_ROW_AUTHENTICATION:
        return new QCheckBox(parent);
    case SOCKET_ROW_OPTION:
    case SOCKET_ROW_ADDRESS:
    case SOCKET_ROW_CHANNEL:
        return new QComboBox(parent);
    case SOCKET_ROW_PORT:
        return new QSpinBox(parent);
    case SOCKET_ROW_USERNAME:
    case SOCKET_ROW_PASSWORD:
    case SOCKET_ROW_DESCRIPTION:
        return new QLineEdit(parent);
    default:
        return nullptr;
    }
}

void SocketTransferDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int column = index.column();
    if (column == SOCKET_ROW_AUTHENTICATION) {
        qobject_cast<QCheckBox *>(editor)->setChecked(index.data(Qt::EditRole).toBool());
    } else if (column == SOCKET_ROW_OPTION || column == SOCKET_ROW_ADDRESS
               || column == SOCKET_ROW_CHANNEL) {
        auto cb = qobject_cast<QComboBox *>(editor);
        if (column == SOCKET_ROW_OPTION) {
            xSetupTransferType(cb);
            int transferType = index.data(Qt::EditRole).toInt();
            int index = cb->findData(transferType);
            if (index == -1) {
                cb->setCurrentIndex(0);
            } else {
                cb->setCurrentIndex(index);
            }
        } else if (column == SOCKET_ROW_ADDRESS) {
            xSetupSocketAddress(cb);
            cb->setEditable(true);
            QString text = index.data(Qt::EditRole).toString();
            cb->setCurrentText(text);
        } else if (column == SOCKET_ROW_CHANNEL) {
            xSetupWebSocketDataChannel(cb);
            int dataChannel = index.data(Qt::EditRole).toInt();
            int index = cb->findData(dataChannel);
            if (index == -1) {
                cb->setCurrentIndex(0);
            } else {
                cb->setCurrentIndex(index);
            }
        }
    } else if (column == SOCKET_ROW_PORT) {
        auto sb = qobject_cast<QSpinBox *>(editor);
        xSetupSocketPort(sb);
        sb->setValue(index.data(Qt::EditRole).toInt());
    } else if (column == SOCKET_ROW_USERNAME || column == SOCKET_ROW_PASSWORD
               || column == SOCKET_ROW_DESCRIPTION) {
        auto le = qobject_cast<QLineEdit *>(editor);
        le->setText(index.data(Qt::EditRole).toString());
    }
}

void SocketTransferDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const
{
    int column = index.column();
    if (column == SOCKET_ROW_AUTHENTICATION) {
        model->setData(index, qobject_cast<QCheckBox *>(editor)->isChecked(), Qt::EditRole);
    } else if (column == SOCKET_ROW_ADDRESS || column == SOCKET_ROW_CHANNEL
               || column == SOCKET_ROW_OPTION) {
        auto cb = qobject_cast<QComboBox *>(editor);
        if (column == SOCKET_ROW_CHANNEL || column == SOCKET_ROW_OPTION) {
            model->setData(index, cb->currentData(), Qt::EditRole);
        } else {
            model->setData(index, cb->currentText());
        }
    } else if (column == SOCKET_ROW_AUTHENTICATION || column == SOCKET_ROW_PORT) {
        auto sb = qobject_cast<QSpinBox *>(editor);
        model->setData(index, sb->value(), Qt::EditRole);
    } else if (column == SOCKET_ROW_USERNAME || column == SOCKET_ROW_PASSWORD
               || column == SOCKET_ROW_DESCRIPTION) {
        auto le = qobject_cast<QLineEdit *>(editor);
        model->setData(index, le->text(), Qt::EditRole);
    } else {
        qWarning() << "SocketTransferDelegate::setModelData: Unsupported column" << column;
    }
}
