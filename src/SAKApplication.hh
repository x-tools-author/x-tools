/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKAPPLICATION_HH
#define SAKAPPLICATION_HH

#include <QTranslator>
#include <QApplication>
#include <QStyleFactory>
#include <QQmlApplicationEngine>

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
     * @brief setupUi 安装/切换ui
     * @param classicalUi 该值为true时，切换为经典ui（基于味道哥特式），否则切换为“现代ui”（基于qml）
     */
    void setupUi(bool classicalUi);
private:
    SAKMainWindow *mainWindow;
    QQmlApplicationEngine *qmlApplicationEngine;

    QTranslator qtTranslator;
    QTranslator qtBaeTranslator;
    QTranslator sakTranslator;
signals:
    void checkForUpdate();
};

#endif
