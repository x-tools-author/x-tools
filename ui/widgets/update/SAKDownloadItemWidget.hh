/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDOWNLOADITEMWIDGET_HH
#define SAKDOWNLOADITEMWIDGET_HH

#include <QLabel>
#include <QWidget>
#include <QPushButton>

namespace Ui {
    class SAKDownloadItemWidget;
}

class SAKDownloadItemWidget:public QWidget
{
    Q_OBJECT
public:
    SAKDownloadItemWidget(QString downloadUrl, QWidget *parent);
    ~SAKDownloadItemWidget();
private:
    Ui::SAKDownloadItemWidget *ui;
    QLabel *fileNameLabel;
    QPushButton *downloadPushButton;

private slots:
    void on_downloadPushButton_clicked();

private:
    QString downloadUrl;
};

#endif
