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

#ifndef CONFIRMUPDATEDIALOG_H
#define CONFIRMUPDATEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QUrl>

namespace Ui {
class ConfirmUpdateDialog;
}

class ConfirmUpdateDialog:public QDialog
{
    Q_OBJECT
public:
    ConfirmUpdateDialog();
    ~ConfirmUpdateDialog();
    void setInfo(QString version, QUrl url, QString description);
    void setCurrentVersion(QString version){oldVersion->setText(version);}
private:
    Ui::ConfirmUpdateDialog* ui = nullptr;
    QLabel* newVersion;
    QLabel* oldVersion;
    QLabel* desc;

    QUrl address;
private slots:
    void need2download(){emit download(address);}
signals:
    void download(QUrl url);
};

#endif
