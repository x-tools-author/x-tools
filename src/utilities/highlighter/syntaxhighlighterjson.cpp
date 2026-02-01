/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "syntaxhighlighterjson.h"

namespace xTools {

class SyntaxHighlighterJsonPrivate : public QObject
{
public:
    explicit SyntaxHighlighterJsonPrivate(SyntaxHighlighterJson *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~SyntaxHighlighterJsonPrivate() = default;

public:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;

private:
    SyntaxHighlighterJson *q{nullptr};
};

SyntaxHighlighterJson::SyntaxHighlighterJson(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    d = new SyntaxHighlighterJsonPrivate(this);
    // Key format
    QTextCharFormat keyFormat;
    keyFormat.setForeground(QColor("#a4ab1e"));
    SyntaxHighlighterJsonPrivate::HighlightingRule keyRule;
    keyRule.pattern = QRegularExpression(R"("([^"\\]|\\.)*"\s*:)");
    keyRule.format = keyFormat;
    d->m_highlightingRules.append(keyRule);
    // String format
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor("#008000"));
    SyntaxHighlighterJsonPrivate::HighlightingRule stringRule;
    stringRule.pattern = QRegularExpression(R"("([^"\\]|\\.)*")");
    stringRule.format = stringFormat;
    d->m_highlightingRules.append(stringRule);
    // Number format
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor("#FF00FF"));
    SyntaxHighlighterJsonPrivate::HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression(R"(\b-?(0x)?\d+(\.\d+)?([eE][+-]?\d+)?\b)");
    numberRule.format = numberFormat;
    d->m_highlightingRules.append(numberRule);
    // Boolean format
    QTextCharFormat booleanFormat;
    booleanFormat.setForeground(QColor("#FF4500"));
    SyntaxHighlighterJsonPrivate::HighlightingRule booleanRule;
    booleanRule.pattern = QRegularExpression(R"(\b(true|false|null)\b)");
    booleanRule.format = booleanFormat;
    d->m_highlightingRules.append(booleanRule);
}

void SyntaxHighlighterJson::highlightBlock(const QString &text)
{
    for (const SyntaxHighlighterJsonPrivate::HighlightingRule &rule :
         const_cast<QVector<SyntaxHighlighterJsonPrivate::HighlightingRule> &>(
             d->m_highlightingRules)) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

} // namespace xTools