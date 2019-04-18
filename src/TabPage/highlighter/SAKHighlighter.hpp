/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef SAKHIGHLIGHTER_HPP
#define SAKHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>

class SAKHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    /**
     * @brief SAKHighlighter    -- 构造函数，nothing to say.
     * @param parent            -- 父控件，用于释放资源
     */
    SAKHighlighter(QTextDocument* parent = nullptr);

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
