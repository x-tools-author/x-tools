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
#ifndef SAKTRANSMISSIONPAGE_HH
#define SAKTRANSMISSIONPAGE_HH

#include <QLabel>
#include <QTimer>
#include <QListWidget>
#include <QPushButton>

class SAKDebugPage;

namespace Ui {
    class SAKTransmissionPage;
}

class SAKTransmissionPage:public QWidget
{
    Q_OBJECT
public:
    SAKTransmissionPage(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~SAKTransmissionPage();

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
    QLabel      *infoLabel;

    SAKDebugPage *_debugPage;
    Ui::SAKTransmissionPage *ui;

    int transmissionType;
    QTimer clearMessageInfoTimer;
private:
    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
private slots:
    void on_addItemPushButton_clicked();
    void on_deleteItemPushButton_clicked();
};

#endif
