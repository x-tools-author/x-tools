/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDHIGHLIGHTER_HPP
#define EDHIGHLIGHTER_HPP

#include <QSyntaxHighlighter>

class EDHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    EDHighlighter(QObject *parent = nullptr);

    Q_INVOKABLE void setDoc(QVariant doc);
    Q_INVOKABLE void setKeyWords(QVariant keyWords);
    Q_INVOKABLE void removeKeyWord(const QString &keyWord);

protected:
    void highlightBlock(const QString &text) final;

private:
    QStringList mKeyWords;
};

#endif // EDHIGHLIGHTER_HPP
