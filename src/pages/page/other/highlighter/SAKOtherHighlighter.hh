/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERHIGHLIGHTER_HH
#define SAKOTHERHIGHLIGHTER_HH

#include <QTextDocument>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class SAKOtherHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SAKOtherHighlighter(QTextDocument* parent = Q_NULLPTR);

    /**
     * @brief setHighlighterKeyWord: Set highligh key word
     * @param keyWords: Key word
     */
    void setHighlighterKeyWord(QStringList keyWords);
private:
    struct HighlightingRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> mHighlightingRules;

    QRegularExpression mCommentStartExpression;
    QRegularExpression mCommentEndExpression;

    QTextCharFormat mKeywordFormat;
    QTextCharFormat mClassFormat;
    QTextCharFormat mSingleLineCommentFormat;
    QTextCharFormat mMultiLineCommentFormat;
    QTextCharFormat mQuotationFormat;
    QTextCharFormat mFunctionFormat;

    void highlightBlock(const QString &text) override;
};

#endif
