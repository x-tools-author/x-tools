/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
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
#ifndef TRANSMISSIONPAGE_HH
#define TRANSMISSIONPAGE_HH

#include <QListWidget>
#include <QPushButton>

class SAKDebugPage;

namespace Ui {
class TransmissionPage;
}

class TransmissionPage:public QWidget
{
    Q_OBJECT
public:
    TransmissionPage(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~TransmissionPage();

    enum TransmissionType {
        SerialPortTransmission,
        TcpTransmission,
        UdpTransmission
    };

    void setTransmissionType(TransmissionType type);
private:
    QPushButton *addItemPushButton;
    QPushButton *deleteItemPushButton;
    QListWidget *listWidget;

    SAKDebugPage *_debugPage;
    Ui::TransmissionPage *ui;

    TransmissionType transmissionType;
private slots:
    void on_addItemPushButton_clicked();
    void on_deleteItemPushButton_clicked();
};

#endif
