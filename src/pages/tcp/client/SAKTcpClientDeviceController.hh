/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPCLIENTDEVICECONTROLLER_HH
#define SAKTCPCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKTcpClientDeviceController;
}

class SAKDebugPage;
class SAKTcpClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    SAKTcpClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTcpClientDeviceController();

    QVariant parameters() final;
    QString localHost();
    quint16 localPort();
    QString serverHost();
    quint16 serverPort();
    bool enableCustomLocalSetting();

    void refresh();
    void setUiEnable(bool enable);
private:
    QMutex uiMutex;
private:
    Ui::SAKTcpClientDeviceController *ui;
    QComboBox *localhostComboBox;
    QLineEdit *localPortlineEdit;
    QCheckBox *enableLocalSettingCheckBox;
    QLineEdit *serverHostLineEdit;
    QLineEdit *serverPortLineEdit;
};

#endif
