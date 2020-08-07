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

/// Packet item
class SAKDownloadItemWidget:public QWidget
{
    Q_OBJECT
public:
    SAKDownloadItemWidget(QString mDownloadUrl, QWidget *parent);
    ~SAKDownloadItemWidget();
private:
    QString mDownloadUrl;
private:
    Ui::SAKDownloadItemWidget *mUi;
    QLabel *mFileNameLabel;
    QPushButton *mDownloadPushButton;
private slots:
    void on_downloadPushButton_clicked();
};

#endif
