#ifndef SAKGUIAPPLICATION_H
#define SAKGUIAPPLICATION_H

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

#endif // SAKGUIAPPLICATION_H
