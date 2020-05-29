/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKMOREOTHERSETTINGS_HH
#define SAKMOREOTHERSETTINGS_HH

#include <QWidget>

namespace Ui {
    class SAKMoreSettingsWidget;
}

class SAKDebugPage;

class SAKMoreSettingsWidget:public QWidget
{
    Q_OBJECT
public:
    SAKMoreSettingsWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKMoreSettingsWidget();
private:
    SAKDebugPage *_debugPage;
    Ui::SAKMoreSettingsWidget *ui;
};

#endif
