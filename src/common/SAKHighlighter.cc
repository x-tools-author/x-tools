/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QDebug>
#include <QTextDocument>
#include <QRegularExpression>
#include <QQuickTextDocument>

#include "SAKHighlighter.hh"

SAKHighlighter::SAKHighlighter(QObject *parent)
    : QSyntaxHighlighter(parent)
{

}

void SAKHighlighter::setDoc(QVariant doc)
{
    if (doc.canConvert<QQuickTextDocument*>()) {
        setDocument(doc.value<QQuickTextDocument*>()->textDocument());
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
