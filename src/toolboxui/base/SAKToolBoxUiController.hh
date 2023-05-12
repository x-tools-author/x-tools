/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUICONTROLLER_HH
#define SAKTOOLBOXUICONTROLLER_HH

#include "tools/SAKCommunicationTool.hh"
#include <QWidget>

class SAKToolBoxUiController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKToolBoxUiController(QWidget *parent = nullptr);

signals:

};

#endif // SAKTOOLBOXUICONTROLLER_H
