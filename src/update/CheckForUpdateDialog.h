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
#ifndef CHECKFORUPDATEDIALOG_H
#define CHECKFORUPDATEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QProgressBar>

class HttpAnalyzer;

namespace Ui {
class CheckForUpdateDialog;
}

class CheckForUpdateDialog:public QDialog
{
    Q_OBJECT
public:
    CheckForUpdateDialog(HttpAnalyzer* httpAnalyzer);
    ~CheckForUpdateDialog();

    void checkForUpdate();
private:
    Ui::CheckForUpdateDialog* ui = nullptr;
    QPushButton* cancelBt = nullptr;
    HttpAnalyzer* analyzer = nullptr;

    QLabel* infoLabel = nullptr;
    QProgressBar* progressBar = nullptr;
private slots:
    void cancel();
    void showErrorDialog(QString errStr);
    void noNewVersion();
signals:
    void cancelChecking();
};

#endif
