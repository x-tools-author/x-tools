/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKFONTAWESOMEASSISTANTDATASTTRUCT_H
#define SAKFONTAWESOMEASSISTANTDATASTTRUCT_H

#include <QString>
#include <QStringList>

struct SAKFontAwesomeIconContext
{
    QString label;
    int unicode;
    QString family;
    QStringList terms;
};

enum SAKFontAwesomeFamily
{
    BrandRegular,
    FreeRegular,
    FreeSolid,
};

#endif // SAKFONTAWESOMEASSISTANTDATASTTRUCT_H
