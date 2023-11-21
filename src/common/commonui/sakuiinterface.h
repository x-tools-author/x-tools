/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKUIINTERFACE_H
#define SAKUIINTERFACE_H

#include <QLineEdit>
#include <QObject>

class SAKUiInterface : public QObject
{
    Q_OBJECT
public:
    explicit SAKUiInterface(QObject *parent = nullptr);

    static void setValidator(QLineEdit *le, int textFormat);
    static QIcon cookedIcon(const QIcon &icon);
};

#endif // SAKUIINTERFACE_H
