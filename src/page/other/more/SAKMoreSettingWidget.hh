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
 */
#ifndef MOREOTHERSETTINGS_HH
#define MOREOTHERSETTINGS_HH

#include <QWidget>

namespace Ui {
    class MoreOtherSettingsWidget;
}

class SAKDebugPage;

class MoreOtherSettingsWidget:public QWidget
{
    Q_OBJECT
public:
    MoreOtherSettingsWidget(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~MoreOtherSettingsWidget();
private:
    SAKDebugPage *_debugPage;
    Ui::MoreOtherSettingsWidget *ui;
};

#endif
