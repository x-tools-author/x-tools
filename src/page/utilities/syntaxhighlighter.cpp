/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "syntaxhighlighter.h"

#include <QTextDocument>

SyntaxHighlighter::SyntaxHighlighter(QObject *parent)
    : QSyntaxHighlighter(parent)
{}

void SyntaxHighlighter::setKeywords(const QStringList &keywords)
{
    m_keywords = keywords;
}

void SyntaxHighlighter::setEnabled(bool enable)
{
    m_enable = enable;
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    if (!m_enable) {
        return;
    }

    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkMagenta);

    for (auto &keyword : m_keywords) {
        auto ret = text.indexOf(keyword);
        if (ret != -1) {
            int length = keyword.length();
            setFormat(ret, length, format);
        }
    }
}
