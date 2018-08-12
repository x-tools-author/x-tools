#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "SAKApplication.h"

#include <QDebug>

SAKApplication::SAKApplication(int argc, char **argv):
    QApplication(argc, argv),
    mpMainWindow(new MainWindow)
{
    mpMainWindow->show();
}

SAKApplication::~SAKApplication()
{

}
