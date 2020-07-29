/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTRANSMISSIONPAGE_HH
#define SAKTRANSMISSIONPAGE_HH

#include <QLabel>
#include <QTimer>
#include <QListWidget>
#include <QPushButton>

class SAKDebugPage;

namespace Ui {
    class SAKOtherTransmissionPage;
}

class SAKOtherTransmissionPage:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTransmissionPage(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherTransmissionPage();

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
    Ui::SAKOtherTransmissionPage *ui;

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
