/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETSERVERTOOL_HH
#define SAKSOCKETSERVERTOOL_HH

#include "SAKBaseTool.hh"

class SAKSocketServerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKSocketServerTool(const char *log, QObject *parent = nullptr);
};

#endif // SAKSOCKETSERVERTOOL_HH
