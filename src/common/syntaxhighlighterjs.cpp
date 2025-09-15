/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "syntaxhighlighterjs.h"

SyntaxHighlighterJs::SyntaxHighlighterJs(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // 关键字
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#0077aa"));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {"\\bif\\b",
                                   "\\bthen\\b",
                                   "\\belse\\b",
                                   "\\belseif\\b",
                                   "\\bend\\b",
                                   "\\bfunction\\b",
                                   "\\breturn\\b",
                                   "\\bfor\\b",
                                   "\\bwhile\\b",
                                   "\\bbreak\\b",
                                   "\\bdo\\b",
                                   "\\blocal\\b",
                                   "\\bin\\b",
                                   "\\btrue\\b",
                                   "\\bfalse\\b",
                                   "\\bnil\\b"};
    for (const QString &pattern : keywordPatterns) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // 单行注释
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor("#888888"));
    HighlightingRule commentRule;
    commentRule.pattern = QRegularExpression("--[^\n]*");
    commentRule.format = commentFormat;
    highlightingRules.append(commentRule);

    // 字符串
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor("#aa5500"));
    HighlightingRule stringRule;
    stringRule.pattern = QRegularExpression("\".*\"|'.*'");
    stringRule.format = stringFormat;
    highlightingRules.append(stringRule);

    // 数字
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor("#009900"));
    HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression("\\b[0-9]+(\\.[0-9]+)?\\b");
    numberRule.format = numberFormat;
    highlightingRules.append(numberRule);
}

void SyntaxHighlighterJs::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
