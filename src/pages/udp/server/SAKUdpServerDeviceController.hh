/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPSERVERDEVICECONTROLLER_HH
#define SAKTCPSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>

namespace Ui {
    class SAKTcpServerDeviceController;
}

class SAKTcpServerDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKTcpServerDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerDeviceController();

    QString serverHost();
    quint16 serverPort();

    QString currentClientHost();
    quint16 currentClientPort();

    void refresh();
    void setUiEnable(bool enable);

    void addClient(QString host, quint16 port, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
private:
    Ui::SAKTcpServerDeviceController *ui;

    QComboBox *serverHostComboBox;
    QLineEdit *serverPortLineEdit;
    QComboBox *clientHostComboBox;
    QMutex uiMutex;
};

#endif
