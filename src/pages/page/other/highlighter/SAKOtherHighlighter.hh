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
    /**
     * @brief SAKHighlighter    -- 构造函数，nothing to say.
     * @param parent            -- 父控件，用于释放资源
     */
    SAKOtherHighlighter(QTextDocument* parent = Q_NULLPTR);

    /**
     * @brief setHighlighterKeyWord -- 设置高亮显示关键字
     * @param keyWords              -- 关键字
     */
    void setHighlighterKeyWord(QStringList keyWords);
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    void highlightBlock(const QString &text) override;
};

#endif
