/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QString>

class TsItem
{
public:
    TsItem(const QString &filePath, int lineNumber, const QString &lineText, TsItem *preItem);
    ~TsItem();

    bool isSource() const;
    bool isTranslation() const;
    bool isFinishedTranslation() const;
    QString text() const;
    QString cookedText() const;
    int lineNumber() const;
    void updateTranslation(const QString &translation);
    QString translationText() const;

    TsItem *preTsItem() const;
    TsItem *nextTsItem() const;
    void setNextTsItem(TsItem *nextItem);

private:
    QString m_filePath;
    int m_lineNumber; // starting from 1
    QString m_text;
    TsItem *m_preTsItem{nullptr};
    TsItem *m_nextTsItem{nullptr};
};