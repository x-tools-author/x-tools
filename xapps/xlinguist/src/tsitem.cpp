/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsitem.h"

#include <QDebug>

TsItem::TsItem(const QString &filePath, int lineNumber, const QString &lineText, TsItem *preItem)
    : m_filePath(filePath)
    , m_lineNumber(lineNumber)
    , m_text(lineText)
    , m_preTsItem(preItem)
    , m_nextTsItem(nullptr)
{}

TsItem::~TsItem() {}

bool TsItem::isSource() const
{
    QString tmp = m_text.trimmed(); // source as: <source>Engine</source>
    if (tmp.startsWith("<source>") && tmp.endsWith("</source>")) {
        return true;
    }

    return false;
}

bool TsItem::isTranslation() const
{
    QString tmp = m_text.trimmed(); // translation as: <translation type="unfinished"></translation>
    if (tmp.startsWith("<translation") && tmp.endsWith("</translation>")) {
        return true;
    }

    return false;
}

bool TsItem::isFinishedTranslation() const
{
    QString tmp = m_text.trimmed(); // translation as: <translation>翻译引擎</translation>
    if (tmp.startsWith("<translation>") && tmp.endsWith("</translation>")) {
        return true;
    }

    return false;
}

QString TsItem::text() const
{
    return m_text;
}

QString TsItem::cookedText() const
{
    if (!isSource()) {
        return QString();
    }

    QString tmp = m_text.trimmed(); // source as: <source>Engine</source>
    tmp.remove("<source>");
    tmp.remove("</source>");
    return tmp;
}

int TsItem::lineNumber() const
{
    return m_lineNumber;
}

void TsItem::updateTranslation(const QString &translation)
{
    // translation as: <translation type="unfinished"></translation>
    // translation as: <translation>翻译引擎</translation>
    QString left = m_text.left(m_text.indexOf('<'));
    QString tmp = left + "<translation>" + translation + "</translation>";
    m_text = tmp;
}

TsItem *TsItem::preTsItem() const
{
    return m_preTsItem;
}

TsItem *TsItem::nextTsItem() const
{
    return m_nextTsItem;
}

void TsItem::setNextTsItem(TsItem *nextItem)
{
    m_nextTsItem = nextItem;
}