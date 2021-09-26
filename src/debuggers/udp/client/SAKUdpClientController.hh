/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTCONTROLLER_HH
#define SAKUDPCLIENTCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include "SAKDebuggerController.hh"
#include "SAKCommonDataStructure.hh"

namespace Ui {
    class SAKUdpClientController;
}

class SAKUdpClientController : public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKUdpClientController(QSettings *settings,
                           const QString &settingsGroup,
                           QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;
    void onClientInfoChanged(QString info);
private:
    Ui::SAKUdpClientController *mUi;
};
#endif
