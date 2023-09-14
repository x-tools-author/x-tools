#ifndef SAKGUIAPPLICATION_HH
#define SAKGUIAPPLICATION_HH

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class SAKGuiApplication : public QGuiApplication
{
    Q_OBJECT
public:
    SAKGuiApplication(int argc, char *argv[]);
    QQmlApplicationEngine &qmlAppEngine();

private:
    QQmlApplicationEngine mQmlAppEngine;
};

#endif // SAKGUIAPPLICATION_HH
