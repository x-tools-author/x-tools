/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETSERVERCONTROLLER_HH
#define SAKWEBSOCKETSERVERCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QWebSocket>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKWebSocketServerController;
}

class SAKWebSocketServerController : public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKWebSocketServerController(QSettings *settings,
                                 const QString &settingsGroup,
                                 QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketServerController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;

    void addClient(QString host, quint16 port, QWebSocket *socket);
    void removeClient(QWebSocket *socket);
    void clearClient();
private:
    void updateClientHostCombox();
private:
    Ui::SAKWebSocketServerController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
    QComboBox *mSendingTypeComboBox;
    int mCurClientIndex;
};
#endif
