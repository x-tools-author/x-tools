/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSyntaxHighlighter>

class xToolsHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    xToolsHighlighter(QObject *parent = nullptr);

    Q_INVOKABLE void setDoc(QVariant doc);
    Q_INVOKABLE void setKeyWords(QVariant keyWords);
    Q_INVOKABLE void removeKeyWord(const QString &keyWord);

protected:
    void highlightBlock(const QString &text) override;

private:
    QStringList mKeyWords;
};
