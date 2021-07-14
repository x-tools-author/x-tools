/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTRANSMISSIONPAGE_HH
#define SAKOTHERTRANSMISSIONPAGE_HH

#include <QLabel>
#include <QTimer>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
    class SAKOtherTransmissionPage;
}

class SAKDebugPage;
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
    void import(const QString fileName);
    void outport(const QString fileName);
private:
    QPushButton *mAddItemPushButton;
    QPushButton *mDeleteItemPushButton;
    QListWidget *mListWidget;
    QLabel*mInfoLabel;

    SAKDebugPage *mDebugPage;
    Ui::SAKOtherTransmissionPage *mUi;

    int mTransmissionType;
    QTimer mClearMessageInfoTimer;
private:
    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
private slots:
    void on_addItemPushButton_clicked();
    void on_deleteItemPushButton_clicked();
    void on_closePushButton_clicked();
    void on_importPushButton_clicked();
    void on_outportPushButton_clicked();
signals:
    void invokeClose();
};

#endif
