/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Common/xExec.h"
#include "FileMergeAssistant.h"

int main(int argc, char* argv[])
{
    const QString appName = QObject::tr("File Merge Assistant");
    return xTools::execCentralWidget<FileMergeAssistant>(argc, argv, appName);
}
