/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
