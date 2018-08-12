#ifndef SAKAPPLICATION_H
#define SAKAPPLICATION_H

#include <QApplication>

#include "mainwindow.h"

class SAKApplication:public QApplication
{
    Q_OBJECT
public:
    SAKApplication(int argc, char **argv);
    ~SAKApplication();
private:
    MainWindow *mpMainWindow;
};

#endif
