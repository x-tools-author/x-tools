/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPCLIENTCONTROLLER_HH
#define SAKTCPCLIENTCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKTcpClientController;
}

class SAKDebugger;
class SAKTcpClientController:public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKTcpClientController(QSettings *settings,
                           const QString &settingsGroup,
                           QWidget *parent = Q_NULLPTR);
    ~SAKTcpClientController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;
    void onServerInfoChanged(QString info);
private:
    QMutex mParametersMutex;
private:
    Ui::SAKTcpClientController *mUi;
    QComboBox *mLocalhostComboBox;
    QLineEdit *mLocalPortlineEdit;
    QCheckBox *mSpecifyClientAddressAndPort;
    QCheckBox *mAutomaticConnectionCheckBox;
    QLineEdit *mClientInfoLineEdit;
    QLineEdit *mServerHostLineEdit;
    QLineEdit *mServerPortLineEdit;
};
#endif
