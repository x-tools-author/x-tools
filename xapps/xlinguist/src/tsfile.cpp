/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfile.h"

#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>

#include "tsitem.h"

TsFile::TsFile(const QString &filePath, QObject *parent)
    : QStandardItemModel(parent)
    , m_filePath(filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return;
    }

    QTextStream in(&file);
    int lineNumber = 0;
    TsItem *preItem = nullptr;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QStandardItem *> items;
        QStandardItem *item = new QStandardItem(line);
        TsItem *tsItem = new TsItem(filePath, ++lineNumber, line, preItem);
        if (preItem) {
            preItem->setNextTsItem(tsItem);
        }
        preItem = tsItem;

        item->setData(QVariant::fromValue(static_cast<void *>(tsItem)), TS_FILE_ITEM_ROLE);
        items.append(item);
        this->appendRow(items);

        // Extract language name form: <TS version="2.1" language="ar">
        if (lineNumber == 3) {
            int index2 = line.lastIndexOf("=");
            if (index2 != -1) {
                int lenOfRigth = line.length() - index2 - 1;
                QString lan = line.right(lenOfRigth);
                lan.remove(QRegularExpression("[\"'> ]"));
                m_targetLanguage = lan;
            }
        }
    }

    connect(this,
            &TsFile::invokeUpdateTranslation,
            this,
            &TsFile::updateTranslationThreadSafe,
            Qt::QueuedConnection);
}

TsFile::~TsFile()
{
    for (int row = 0; row < this->rowCount(); ++row) {
        QStandardItem *item = this->item(row);
        QVariant var = item->data(TS_FILE_ITEM_ROLE);
        TsItem *tsItem = static_cast<TsItem *>(var.value<void *>());
        delete tsItem;
    }
}

QString TsFile::baseName() const
{
    QFileInfo fileInfo(m_filePath);
    return fileInfo.baseName();
}

QString TsFile::targetLanguage() const
{
    return m_targetLanguage;
}

QString TsFile::translationStatus() const
{
    QList<TsItem *> itemList = tsItems();
    int totalItems = 0;
    int finishedItems = 0;
    for (TsItem *item : itemList) {
        if (item->isTranslation()) {
            ++totalItems;
            if (item->isFinishedTranslation()) {
                ++finishedItems;
            }
        }
    }

    return QString("%1/%2").arg(finishedItems).arg(totalItems);
}

QString TsFile::filePath() const
{
    return m_filePath;
}

QList<TsItem *> TsFile::tsItems() const
{
    QList<TsItem *> itemList;
    for (int row = 0; row < this->rowCount(); ++row) {
        QStandardItem *item = this->item(row);
        QVariant var = item->data(TS_FILE_ITEM_ROLE);
        TsItem *tsItem = static_cast<TsItem *>(var.value<void *>());
        itemList.append(tsItem);
    }
    return itemList;
}

void TsFile::updateTranslation(const QString &translation, int sourceLineNumber)
{
    emit invokeUpdateTranslation(translation, sourceLineNumber);
}

void TsFile::updateTranslationThreadSafe(const QString &translation, int sourceLineNumber)
{
    QStandardItem *sourceItem = this->item(sourceLineNumber - 1);
    if (!sourceItem) {
        qWarning() << "No standard item found for line number:" << sourceLineNumber;
        return;
    }

    TsItem *sourceTsItem = static_cast<TsItem *>(
        sourceItem->data(TS_FILE_ITEM_ROLE).value<void *>());
    if (!sourceTsItem) {
        qWarning() << "No TsItem found for line number:" << sourceLineNumber;
        return;
    }

    TsItem *translationItem = sourceTsItem->nextTsItem();
    if (!(translationItem && translationItem->isTranslation())) {
        qWarning() << "No translation item found for source line number:" << sourceLineNumber;
        return;
    }

    translationItem->updateTranslation(translation);
    QStandardItem *stdTranslationItem = this->item(sourceLineNumber + 1); // next line
    if (!stdTranslationItem) {
        qWarning() << "No standard translation item found for line number:"
                   << (sourceLineNumber + 1);
        return;
    }

    stdTranslationItem->setText(translationItem->text());
}