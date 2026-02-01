/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "syntaxhighlighterlua.h"

namespace xTools {

class SyntaxHighlighterLuaPrivate
{
public:
    explicit SyntaxHighlighterLuaPrivate(SyntaxHighlighterLua *q_ptr)
        : q(q_ptr)
    {}
    ~SyntaxHighlighterLuaPrivate() = default;

public:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;

private:
    SyntaxHighlighterLua *q{nullptr};
};

SyntaxHighlighterLua::SyntaxHighlighterLua(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    d = new SyntaxHighlighterLuaPrivate(this);
    // 关键字
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(0x00, 0x77, 0xaa));
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
        SyntaxHighlighterLuaPrivate::HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        d->m_highlightingRules.append(rule);
    }

    // 单行注释
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor(0x88, 0x88, 0x88));
    SyntaxHighlighterLuaPrivate::HighlightingRule commentRule;
    commentRule.pattern = QRegularExpression("--[^\n]*");
    commentRule.format = commentFormat;
    d->m_highlightingRules.append(commentRule);

    // 字符串
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(0xaa, 0x55, 0x00));
    SyntaxHighlighterLuaPrivate::HighlightingRule stringRule;
    stringRule.pattern = QRegularExpression("\".*\"|'.*'");
    stringRule.format = stringFormat;
    d->m_highlightingRules.append(stringRule);

    // 数字
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor(0x00, 0x99, 0x00));
    SyntaxHighlighterLuaPrivate::HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression("\\b[0-9]+(\\.[0-9]+)?\\b");
    numberRule.format = numberFormat;
    d->m_highlightingRules.append(numberRule);
}

void SyntaxHighlighterLua::highlightBlock(const QString &text)
{
    for (const SyntaxHighlighterLuaPrivate::HighlightingRule &rule :
         const_cast<QVector<SyntaxHighlighterLuaPrivate::HighlightingRule> &>(
             d->m_highlightingRules)) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

} // namespace xTools