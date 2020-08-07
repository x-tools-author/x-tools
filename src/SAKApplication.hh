/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKAPPLICATION_HH
#define SAKAPPLICATION_HH

#include <QTranslator>
#include <QApplication>
#include <QStyleFactory>

class SAKMainWindow;
class SAKApplication:public QApplication
{
    Q_OBJECT
public:
    explicit SAKApplication(int argc, char **argv);
    ~SAKApplication();

    /**
     * @brief installLanguage: Setup the language packet of application.
     * the interface must be called before initalizing any text.
     */
    void installLanguage();

    /**
     * @brief mainWindow: Get the instance of main window
     * @return Instance of main window
     */
    SAKMainWindow *mainWindow();
private:
    QTranslator mQtTranslator;
    QTranslator mQtBaseTranslator;
    QTranslator mSakTranslator;
    SAKMainWindow *mMainWindow;
};

#endif
