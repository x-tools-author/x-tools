/*******************************************************************************
 * Copyright 2023 wuuhaii(qsaker@foxmail.com). All rights reserved.
 ******************************************************************************/
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
    void highlightBlock(const QString &text) final;

private:
    QStringList mKeyWords;
};

#endif // SAKHIGHLIGHTER_H
