/******************************************************************************
 * Copyright 2022 wuhai(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALCONTROLLER_HH
#define SAKBLECENTRALCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKBleCentralController;
}

class SAKBleCentralController : public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKBleCentralController(QSettings *settings,
                            const QString &settingsGroup,
                            QWidget *parent = Q_NULLPTR);
    ~SAKBleCentralController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;
    void setServices(const QStringList &services);
private:
    struct SAKStructSettingsKeyContext {
        QString portName;
        QString frameInterval;
        QString usingCustomBaudRate;
        QString baudRate;
    }mSettingsKeyContext;
private:
    Ui::SAKBleCentralController *mUi;
    const int defauleFrameInterval;
};
#endif
