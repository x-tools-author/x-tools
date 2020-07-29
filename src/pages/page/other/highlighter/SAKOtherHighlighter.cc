/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKOtherHighlighter.hh"

SAKOtherHighlighter::SAKOtherHighlighter(QTextDocument* parent)
    :QSyntaxHighlighter (parent)
{
    setHighlighterKeyWord(QStringList());
}

void SAKOtherHighlighter::setHighlighterKeyWord(QStringList keyWords)
{
    mHighlightingRules.clear();

    HighlightingRule rule;

    mKeywordFormat.setForeground(Qt::red);
    mKeywordFormat.setFontWeight(QFont::Normal);

    foreach (const QString &pattern, keyWords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = mKeywordFormat;
        mHighlightingRules.append(rule);
    }

    mMultiLineCommentFormat.setForeground(Qt::red);

    mCommentStartExpression = QRegularExpression("/\\*");
    mCommentEndExpression = QRegularExpression("\\*/");
}

void SAKOtherHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, mHighlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1){
        startIndex = text.indexOf(mCommentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = mCommentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, mMultiLineCommentFormat);
        startIndex = text.indexOf(mCommentStartExpression, startIndex + commentLength);
    }
}
