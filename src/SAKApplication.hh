/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    SAKApplication(int argc, char **argv);
    ~SAKApplication();

    /**
     * @brief installLanguage 安装语言包
     */
    void installLanguage();

    /**
     * @brief mainWindow 获取主窗口实例指针
     * @return 主窗口实例指针
     */
    SAKMainWindow *mainWindow();
private:
    QTranslator mQtTranslator;
    QTranslator mQtBaseTranslator;
    QTranslator mSakTranslator;
    SAKMainWindow *mMainWindow;
};

#endif
