/*
 * Copyright 2019-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
