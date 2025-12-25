#include "./application.h"
#include "./mainwindow.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setWindowTitle("xTools");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}