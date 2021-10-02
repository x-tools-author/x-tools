/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPSERVERCONTROLLER_HH
#define SAKUDPSERVERCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QVector>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKUdpServerController;
}

/// @brief Udp server control panel
class SAKUdpServerController:public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKUdpServerController(QSettings *settings,
                           const QString &settingsGroup,
                           QWidget *parent = Q_NULLPTR);
    ~SAKUdpServerController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;

    void onAddClient(QString host, quint16 port);
private:
    Ui::SAKUdpServerController *mUi;
};
#endif
