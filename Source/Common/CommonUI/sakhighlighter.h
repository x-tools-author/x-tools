/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKHIGHLIGHTER_H
#define SAKHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SAKHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SAKHighlighter(QObject *parent = nullptr);

    Q_INVOKABLE void setDoc(QVariant doc);
    Q_INVOKABLE void setKeyWords(QVariant keyWords);
    Q_INVOKABLE void removeKeyWord(const QString &keyWord);

protected:
    void highlightBlock(const QString &text) override;

private:
    QStringList mKeyWords;
};

#endif // SAKHIGHLIGHTER_H
