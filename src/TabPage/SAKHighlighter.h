/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKHIGHLIGHTER_H
#define SAKHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SAKHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SAKHighlighter(QObject* parent = nullptr);
    ~SAKHighlighter();
};

#endif
