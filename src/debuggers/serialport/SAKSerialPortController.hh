/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKSERIALPORTCONTROLLER_HH
#define SAKSERIALPORTCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

#include "SAKDebuggerController.hh"
#include "SAKCommonDataStructure.hh"

namespace Ui {
    class SAKSerialPortController;
}

class SAKSerialPortController : public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKSerialPortController(QSettings *settings,
                            const QString &settingsGroup,
                            QWidget *parent = Q_NULLPTR);
    ~SAKSerialPortController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;
private:
    struct SAKStructSettingsKeyContext {
        QString portName;
        QString frameInterval;
        QString usingCustomBaudRate;
        QString baudRate;
    }mSettingsKeyContext;
private:
    Ui::SAKSerialPortController *mUi;
    const int defauleFrameInterval;
};
#endif
