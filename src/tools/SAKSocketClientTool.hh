/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTOOL_HH
#define SAKSOCKETCLIENTTOOL_HH

#include "SAKBaseTool.hh"

class SAKSocketClientTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKSocketClientTool(const char *lg, QObject *parent = nullptr);
};

#endif // SAKSOCKETCLIENTTOOL_HH
