/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
