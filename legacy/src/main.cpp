#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Legacy Application");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}