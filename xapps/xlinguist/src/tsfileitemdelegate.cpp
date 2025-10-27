/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfileitemdelegate.h"

#include <QLineEdit>

#include "tsfile.h"
#include "tsfilefilter.h"
#include "tsitem.h"

TsFileItemDelegate::TsFileItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

TsFileItemDelegate::~TsFileItemDelegate() {}

QWidget *TsFileItemDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    const TsFileFilter *tsFileFilter = qobject_cast<const TsFileFilter *>(index.model());
    if (!tsFileFilter) {
        qWarning() << "Failed to cast model to TsFile.";
        return nullptr;
    }

    int lineIndex = index.row();
    const TsFile *tsFile = qobject_cast<const TsFile *>(tsFileFilter->sourceModel());
    TsItem *tsItem = tsFile->tsItemAtLine(lineIndex);
    if (!tsItem) {
        qWarning() << "Failed to get TsItem at line:" << lineIndex + 1;
        return nullptr;
    }

    if (tsItem->isTranslation()) {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }

    qWarning() << "Not a translation item at line:" << lineIndex + 1;
    return nullptr;
}

void TsFileItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const TsFileFilter *tsFileFilter = qobject_cast<const TsFileFilter *>(index.model());
    if (!tsFileFilter) {
        qWarning() << "Failed to cast model to TsFile.";
        return;
    }

    const TsFile *tsFile = qobject_cast<const TsFile *>(tsFileFilter->sourceModel());
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit && tsFile) {
        TsItem *tsItem = tsFile->tsItemAtLine(index.row());
        if (tsItem) {
            lineEdit->setText(tsItem->translationText());
        }
    }
}

void TsFileItemDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    const TsFileFilter *tsFileFilter = qobject_cast<const TsFileFilter *>(index.model());
    if (!tsFileFilter) {
        qWarning() << "Failed to cast model to TsFile.";
        return;
    }

    TsFile *tsFile = qobject_cast<TsFile *>(tsFileFilter->sourceModel());
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit && tsFile) {
        TsItem *tsItem = tsFile->tsItemAtLine(index.row());
        if (tsItem) {
            tsFile->updateTranslation(lineEdit->text(), tsItem->preTsItem()->lineNumber());
        }
    }
}