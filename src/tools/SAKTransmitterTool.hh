/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTRANSMITTERTOOL_HH
#define SAKTRANSMITTERTOOL_HH

#include "SAKTableModelTool.hh"

class SAKTransmitterTool : public SAKTableModelTool
{
    Q_OBJECT
public:
    explicit SAKTransmitterTool(const char *logCategory,
                                QObject *parent = nullptr);
};

#endif // SAKTRANSMITTERTOOL_HH
