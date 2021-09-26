/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPSERVERCONTROLLER_HH
#define SAKTCPSERVERCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>
#include <QStandardItemModel>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKTcpServerController;
}

class SAKTcpServerController:public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKTcpServerController(QSettings *settings,
                           const QString &settingsGroup,
                           QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;

    void onAddClient(QString host, quint16 port, QTcpSocket *socket);
    void onRemoveClient(QTcpSocket *socket);
private:
    Ui::SAKTcpServerController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
};
#endif
