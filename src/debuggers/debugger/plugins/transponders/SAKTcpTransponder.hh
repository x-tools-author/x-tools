/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKTCPTRANSPONDER_HH
#define SAKTCPTRANSPONDER_HH

#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginTransponder.hh"

namespace Ui {
    class SAKTcpTransponder;
}

class SAKTcpClientDevice;
class SAKTcpTransponder : public SAKDebuggerPluginTransponder
{
    Q_OBJECT
public:
    SAKTcpTransponder(QWidget *parent = Q_NULLPTR);
    SAKTcpTransponder(quint64 id,
                      SAKTcpClientParametersContext parasCtx,
                      QWidget *parent = Q_NULLPTR);
    ~SAKTcpTransponder();
    QVariant parametersContext() final;
private:
    SAKDebuggerDevice *device() final;
    void onDeviceStateChanged(bool opened) final;
    void initSignals();
private:
    Ui::SAKTcpTransponder *mUi;
    SAKTcpClientDevice *mDevice;
};

#endif // SAKTCPTRANSPONDER_HH
