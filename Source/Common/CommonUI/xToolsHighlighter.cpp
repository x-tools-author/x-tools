/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDebug>
#include <QRegularExpression>
#include <QTextDocument>
#ifdef X_TOOLS_ENABLE_MODULE_QML
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include <QQuickTextDocument>
#endif
#endif

#include "xToolsHighlighter.h"

xToolsHighlighter::xToolsHighlighter(QObject *parent)
    : QSyntaxHighlighter(parent)
{}

void xToolsHighlighter::setDoc(QVariant doc)
{
    auto obj = doc.value<QObject *>();
#ifdef X_TOOLS_ENABLE_MODULE_QML
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    if (obj->inherits("QQuickTextDocument")) {
        setDocument(doc.value<QQuickTextDocument *>()->textDocument());
        return;
    }
#endif
#endif
    if (obj->inherits("QTextDocument")) {
        setDocument(doc.value<QTextDocument *>());
    }
}

void xToolsHighlighter::setKeyWords(QVariant keyWords)
{
    QList<QVariant> cookedKeyWords = keyWords.toList();
    mKeyWords.clear();
    for (auto &keyWord : cookedKeyWords) {
        mKeyWords.append(keyWord.toString());
    }
}

void xToolsHighlighter::removeKeyWord(const QString &keyWord)
{
    if (keyWord.isEmpty()) {
        mKeyWords.clear();
        return;
    }

    mKeyWords.removeOne(keyWord);
}

void xToolsHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::darkMagenta);

    for (auto &keyWord : mKeyWords) {
        auto ret = text.indexOf(keyWord);
        if (ret != -1) {
            int length = keyWord.length();
            setFormat(ret, length, format);
        }
    }
}
