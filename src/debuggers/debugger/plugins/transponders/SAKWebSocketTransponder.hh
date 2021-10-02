/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKWEBSOCKETTRANSPONDER_HH
#define SAKWEBSOCKETTRANSPONDER_HH

#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginTransponder.hh"

namespace Ui {
    class SAKWebSocketTransponder;
}

class SAKWebSocketClientDevice;
class SAKWebSocketTransponder : public SAKDebuggerPluginTransponder
{
    Q_OBJECT
public:
    SAKWebSocketTransponder(QWidget *parent = Q_NULLPTR);
    SAKWebSocketTransponder(quint64 id,
                            SAKWSClientParametersContext parasCtx,
                            QWidget *parent = Q_NULLPTR);
    QVariant parametersContext() final;
private:
    SAKDebuggerDevice *device() final;
    void onDeviceStateChanged(bool opened) final;
    void initSignals();
private:
    Ui::SAKWebSocketTransponder *mUi;
    SAKWebSocketClientDevice *mDevice;
};

#endif // SAKWEBSOCKETTRANSPONDER_HH
