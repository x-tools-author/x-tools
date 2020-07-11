/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    SAKTransmissionPage(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
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
