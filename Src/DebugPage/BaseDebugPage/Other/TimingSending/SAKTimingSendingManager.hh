/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef SAKTIMINGSENDINGMANAGER_HH
#define SAKTIMINGSENDINGMANAGER_HH

#include "SAKDebugPage.hh"

#include <QRegExp>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRegExpValidator>

class SAKDebugPage;

namespace Ui {
    class SAKTimingSendingManager;
}

class SAKTimingSendingManager:public QWidget
{
    Q_OBJECT
public:
    SAKTimingSendingManager(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTimingSendingManager();

private:
    Ui::SAKTimingSendingManager *ui;

private:
    SAKDebugPage *debugPage;
signals:
    void write(QByteArray data);
};

#endif
