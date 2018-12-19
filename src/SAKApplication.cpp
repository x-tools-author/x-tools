/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SAKApplication.h"

#include <QDebug>

SAKApplication::SAKApplication(int argc, char **argv):
    QApplication(argc, argv)
    ,sakSettings(new SAKSettings)   /// 必须优先初始化该成员，否则某些功能蒋失效甚至是程序崩溃
{
    QApplication::setStyle(QStyleFactory::create(QString("%1").arg(SAKSettings::valueApplicationStyle())));

    mpMainWindow = new MainWindow;
    mpMainWindow->show();
}

SAKApplication::~SAKApplication()
{

}
