/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QStandardItemModel>

#include "tsitem.h"

#define TS_FILE_ITEM_ROLE (Qt::UserRole + 1)

class TsFile : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TsFile(const QString &filePath, QObject *parent = nullptr);
    ~TsFile() override;

    QString baseName() const;
    QString targetLanguage() const;
    QString translationStatus() const;
    QString filePath() const;
    bool saveToFile(const QString &filePath = QString());

    QList<TsItem *> tsItems() const;
    TsItem *tsItemAtLine(int lineIndex) const;
    void updateTranslation(const QString &translation, int sourceLineNumber);
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    Q_SIGNAL void invokeUpdateTranslation(const QString &translation, int sourceLineNumber);

private:
    QString m_filePath;
    QString m_targetLanguage;

private:
    void updateTranslationThreadSafe(const QString &translation, int sourceLineNumber);
};