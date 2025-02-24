/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SyntaxHighlighter(QObject *parent = nullptr);

    void setKeywords(const QStringList &keywords);
    void setEnabled(bool enable);

protected:
    void highlightBlock(const QString &text) override;

private:
    QStringList m_keywords;
    bool m_enable{false};
};
