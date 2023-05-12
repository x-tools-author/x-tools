/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUIBASECONTROLLER_HH
#define SAKTOOLBOXUIBASECONTROLLER_HH

#include "tools/SAKCommunicationTool.hh"
#include <QWidget>

class SAKToolBoxUiBaseController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKToolBoxUiBaseController(
        SAKCommunicationTool *tool, QWidget *parent = nullptr);

signals:

};

#endif // SAKTOOLBOXUIBASECONTROLLER_HH
