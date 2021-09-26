/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKTESTDEBUGGERCONTROLLER_HH
#define SAKTESTDEBUGGERCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebuggerController.hh"

namespace Ui {
    class SAKTestDebuggerController;
}

class SAKTestDebuggerController : public SAKDebuggerController
{
    Q_OBJECT
public:
    SAKTestDebuggerController(QSettings *settings,
                              const QString &settingsGroup,
                              QWidget *parent = Q_NULLPTR);
    ~SAKTestDebuggerController();

    void updateUiState(bool opened) final;
    void refreshDevice() final;
    QVariant parametersContext() final;
private:
    Ui::SAKTestDebuggerController *mUi;
signals:
    void openFailedChanged(bool openFailed);
    void readCircularlyChanged(bool readCircularly);
    void readIntervalChanged(int interval);
    void writeCircularlyChanged(bool writeCircularly);
    void writtingIntervalChanged(int writtingInterval);
    void errorStringChanged(const QString &errorString);
};
#endif
