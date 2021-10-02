/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERDEVICEMASK_HH
#define SAKDEBUGGERDEVICEMASK_HH

#include <QDialog>
#include <QSettings>

namespace Ui {
    class SAKDebuggerDeviceMask;
}

class SAKDebuggerDeviceMask : public QDialog
{
    Q_OBJECT
public:
    SAKDebuggerDeviceMask(QSettings *settings,
                           QString settingGroup,
                           QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerDeviceMask();

    struct SAKStructMaskContext {
        quint8 rx;
        quint8 tx;
        bool enableRx;
        bool enableTx;
    };

    SAKStructMaskContext parametersContext();
private:
    struct QMDStructSettingsKeyContext {
        QString rxMask;
        QString txMask;
    } mSettingsKeyCtx;
private:
    Ui::SAKDebuggerDeviceMask *mUi;
signals:
    void parametersChanged();
};

#endif // SAKDEBUGGERDEVICEMASK_H
