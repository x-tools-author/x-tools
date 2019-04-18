/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SAKHighlighter.hpp"

SAKHighlighter::SAKHighlighter(QTextDocument* parent)
    :QSyntaxHighlighter (parent)
{
    /// 该功能请参考Qt语法高亮显示示例
    setHighlighterKeyWord(QStringList());
}

void SAKHighlighter::setHighlighterKeyWord(QStringList keyWords)
{
    highlightingRules.clear();

    HighlightingRule rule;

    keywordFormat.setForeground(Qt::red);
    keywordFormat.setFontWeight(QFont::Normal);

    foreach (const QString &pattern, keyWords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SAKHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1){
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
