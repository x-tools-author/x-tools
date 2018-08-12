#include "SAKApplication.h"
#include "console.h"

#include <QMessageLogContext>
#include <QDateTime>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>

void SAKAppMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Console *pConsole = appConsole();
    if (pConsole == NULL){
        return;
    }

    QString info = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ") + QString(context.file) + QString(context.function) + QString::number(context.line) + " " + msg;
    info.remove("\"");
    switch (type) {
    case QtDebugMsg:
        info = "[D ]" + info;
        pConsole->OutputInfo(info);
        break;
    case QtInfoMsg:
        info = "[I ]" + info;
        pConsole->OutputInfo(info);
        break;
    case QtWarningMsg:
        info = "[W ]" + info;
        pConsole->OutputInfo(QString("<font color=red>%1</font>").arg(info));
        break;
    case QtCriticalMsg:
        info = "[C ]" + info;
        pConsole->OutputInfo(QString("<font color=red>%1</font>").arg(info));
        break;
    case QtFatalMsg:
        info = "[F ]" + info;
        pConsole->OutputInfo(QString("<font color=red>%1</font>").arg(info));
        abort();
        break;
    default:
        info = "[? ]" + info;
        pConsole->OutputInfo(QString("<font color=red>%1</font>").arg(info));
        break;
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(SAKAppMsgOutput);
    SAKApplication a(argc, argv);

    return a.exec();
}
