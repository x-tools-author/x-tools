/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETCLIENTCONTROLLER_HH
#define SAKWEBSOCKETCLIENTCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKWebSocketClientController;
}

/// @brief web socket client control panel
class SAKWebSocketClientController : public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKWebSocketClientController(QSettings *settings,
                                 const QString &settingsGroup,
                                 QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketClientController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;

    void onClientInfoChanged(QString info);
private:
    Ui::SAKWebSocketClientController *mUi;
    QLineEdit *mServerAddressLineEdit;
    QComboBox *mSendingTypeComboBox;
    QLineEdit *mClientInfoLineEdit;
};
#endif
