/***************************************************************************************************
 * Copyright 2018-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKAPPLICATION_H
#define SAKAPPLICATION_H

#include <QApplication>
#include <QSplashScreen>

class SAKApplication : public QApplication
{
    Q_OBJECT
public:
    explicit SAKApplication(int argc, char **argv);
    ~SAKApplication() override = default;

    void showSplashScreenMessage(const QString &msg);

signals:
    void activeMainWindow();

private:
    QSplashScreen *m_splashScreen{nullptr};

private:
    static void setupPalette(const QString &fileName);
};

#endif
