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
#ifndef SAKMOREINFORMATION_HH
#define SAKMOREINFORMATION_HH

#include <QDialog>
#include <QTextBrowser>

namespace Ui {
    class SAKMoreInformation;
};

class SAKMoreInformation:public QDialog
{
    Q_OBJECT
public:
    SAKMoreInformation(QWidget* parent = nullptr);
private:
    Ui::SAKMoreInformation* ui = nullptr;
    QTextBrowser *textBrowserHistory;
};

#endif
