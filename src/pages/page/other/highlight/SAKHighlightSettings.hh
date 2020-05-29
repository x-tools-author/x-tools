/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKHIGHLIGHTSETTINGS_HH
#define SAKHIGHLIGHTSETTINGS_HH

#include <QTextDocument>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class SAKHighlightSettings : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    /**
     * @brief SAKHighlighter    -- 构造函数，nothing to say.
     * @param parent            -- 父控件，用于释放资源
     */
    SAKHighlightSettings(QTextDocument* parent = Q_NULLPTR);

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
