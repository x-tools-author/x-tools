/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKAPPLICATION_HH
#define SAKAPPLICATION_HH

#include <QApplication>
#include <QStyleFactory>

class SAKMainWindow;

class SAKApplication:public QApplication
{
    Q_OBJECT
public:
    SAKApplication(int argc, char **argv);
    ~SAKApplication();
private:
    SAKMainWindow *mpMainWindow    = nullptr;
};

#endif
