/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "MainWindow.h"
#include "xApp/Source/xApp.h"
#include "xLicense/Source/PayJsApi.h"

int main(const int argc, char *argv[])
{
    Q_INIT_RESOURCE(xTools);
    Q_INIT_RESOURCE(xApp);

    xApp::PayJsApi::singleton().setPrice(10 * 10 * 10);
    return xApp::exec<xSvg::MainWindow>(argc,
                                        argv,
                                        QString("xSvg"),
                                        QString(X_SVG_VERSION),
                                        QString("50263Qsaker2018.xSvg"));
}
