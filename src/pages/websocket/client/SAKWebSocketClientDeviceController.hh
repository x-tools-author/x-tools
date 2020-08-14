/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWebSocketClientDeviceController_HH
#define SAKWebSocketClientDeviceController_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKWebSocketClientDeviceController;
}

class SAKDebugPage;
/// @brief web socket 客户端控制类
class SAKWebSocketClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    SAKWebSocketClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketClientDeviceController();

    QVariant parameters() final;
    QString serverAddress();
    void setUiEnable(bool enable);
    quint32 sendingType();
private:
    QMutex uiMutex;
private:
    Ui::SAKWebSocketClientDeviceController *ui;
    QLineEdit *serverAddressLineEdit;
    QComboBox *sendingTypeComboBox;
};

#endif
