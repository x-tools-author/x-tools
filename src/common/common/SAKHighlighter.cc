/*********************************************************************************
 * Copyright 2023 wuuhaii(qsaker@foxmail.com). All rights reserved.
 ******************************************************************************/
#include <QDebug>
#include <QTextDocument>
#include <QRegularExpression>
#ifdef SAK_IMPORT_MODULE_QML
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include <QQuickTextDocument>
#endif
#endif

#include "SAKHighlighter.h"

SAKHighlighter::SAKHighlighter(QObject *parent)
    : QSyntaxHighlighter(parent)
{

}

void SAKHighlighter::setDoc(QVariant doc)
{
    auto obj = doc.value<QObject*>();
#ifdef SAK_IMPORT_MODULE_QML
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    if (obj->inherits("QQuickTextDocument")) {
        setDocument(doc.value<QQuickTextDocument*>()->textDocument());
        return;
    }
#endif
#endif
    if (obj->inherits("QTextDocument")) {
        setDocument(doc.value<QTextDocument*>());
    }
}

void SAKHighlighter::setKeyWords(QVariant keyWords)
{
    QList<QVariant> cookedKeyWords = keyWords.toList();
    mKeyWords.clear();
    for (auto &keyWord : cookedKeyWords) {
        mKeyWords.append(keyWord.toString());
    }
}

void SAKHighlighter::removeKeyWord(const QString &keyWord)
{
    if (keyWord.isEmpty()) {
        mKeyWords.clear();
        return;
    }

    mKeyWords.removeOne(keyWord);
}

void SAKHighlighter::highlightBlock(const QString &text)
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
