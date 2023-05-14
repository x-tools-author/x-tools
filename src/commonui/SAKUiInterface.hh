/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUIINTERFACE_HH
#define SAKUIINTERFACE_HH

#include <QObject>
#include <QLineEdit>

class SAKUiInterface : public QObject
{
    Q_OBJECT
public:
    explicit SAKUiInterface(QObject *parent = nullptr);

    static void setValidator(QLineEdit *le, int textFormat);
};

#endif // SAKUIINTERFACE_HH
