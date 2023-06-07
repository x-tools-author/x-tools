/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKPREFERENCES_HH
#define SAKPREFERENCES_HH

#include <QObject>

class SAKPreferences : public QObject
{
    Q_OBJECT
public:
    explicit SAKPreferences(QObject *parent = nullptr);

signals:

};

#endif // SAKPREFERENCES_HH
