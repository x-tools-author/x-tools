/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
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
