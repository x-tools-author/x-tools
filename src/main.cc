/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QScreen>
#include <QPushButton>
#include <QGridLayout>
#include <QSplashScreen>
#include <QDesktopWidget>

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"

int main(int argc, char *argv[])
{
    int exitCode = 0;

    // The application can be reboot.
    do {
        SAKApplication app(argc, argv);
        if (app.instanceIsExisted()){
            QDialog dialog;
            QLabel *tip1 = new QLabel(QObject::tr("SAK is running, the starting will be terminated."), &dialog);
            QLabel *tip2 = new QLabel(QObject::tr("You can new debugging window or page from \"File\" menu."), &dialog);
            QLabel *emptyLabel = new QLabel(QString(" "), &dialog);
            QPushButton *okButton = new QPushButton(QObject::tr("OK"), &dialog);
            QGridLayout *gridLayout = new QGridLayout(&dialog);

            emptyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::reject);
            dialog.setModal(true);
            dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);
            dialog.show();
            QApplication::beep();

            gridLayout->addWidget(tip1, 0, 0, 1, 2);
            gridLayout->addWidget(tip2, 1, 0, 1, 2);
            gridLayout->addWidget(emptyLabel, 2, 0, 1, 1);
            gridLayout->addWidget(okButton, 2, 1, 1, 1);

            dialog.exec();
            app.setSharedMemoryValue(1);
            return -1024;
        }

        // Setup main window
        app.showSplashScreenMessage(QObject::tr("Initializing main window..."));
        SAKMainWindow mainWindow;
        QObject::connect(&app, &SAKApplication::activeMainWindow, &mainWindow, &SAKMainWindow::activateWindow);
        mainWindow.show();

        // Move the main window to the central of desktop.
        QDesktopWidget *desktop = QApplication::desktop();
        int currentScreen = desktop->screenNumber(&mainWindow);
        QList<QScreen*> screenList = QGuiApplication::screens();
        QScreen *screen = screenList.at(currentScreen);
        mainWindow.move((screen->geometry().width() - mainWindow.width())/2, (screen->geometry().height() - mainWindow.height())/2);
        app.showSplashScreenMessage(QObject::tr("Finished..."));

        // Close the splash screen.
        QSplashScreen *splashScreen = app.splashScreen();
        splashScreen->finish(&mainWindow);

        // If exit code is SAK_REBOOT_CODE(1314), The application will reboot.
        exitCode = app.exec();
        delete screen;
    }while (exitCode == SAK_REBOOT_CODE);

    return exitCode;
}
