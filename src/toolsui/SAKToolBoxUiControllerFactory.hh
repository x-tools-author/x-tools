/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUICONTROLLERFACTORY_HH
#define SAKTOOLBOXUICONTROLLERFACTORY_HH

#include <QObject>
#include "tools/SAKCommunicationTool.hh"

class SAKToolBoxUiControllerFactory : public QObject
{
    Q_OBJECT
private:
    explicit SAKToolBoxUiControllerFactory(QObject *parent = nullptr);

public:
    QWidget *createController(int type, SAKCommunicationTool *tool);
    static SAKToolBoxUiControllerFactory *instance();

private:

};

#endif // SAKTOOLBOXUICONTROLLERFACTORY_H
