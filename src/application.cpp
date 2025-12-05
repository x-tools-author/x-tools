/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"

Application::Application(int &argc, char **argv)
    : xApp(argc, argv)
{}

QString Application::left2rightTips() const
{
    return tr("Transfer rx bytes with right page");
}

QString Application::right2leftTips() const
{
    return tr("Transfer rx bytes with left page");
}
