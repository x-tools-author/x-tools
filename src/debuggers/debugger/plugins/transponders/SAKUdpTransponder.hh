/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKUDPTRANSPONDER_HH
#define SAKUDPTRANSPONDER_HH

#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginTransponder.hh"

namespace Ui {
    class SAKUdpTransponder;
}

class SAKUdpClientDevice;
class SAKUdpTransponder : public SAKDebuggerPluginTransponder
{
    Q_OBJECT
public:
    SAKUdpTransponder(QWidget *parent = Q_NULLPTR);
    SAKUdpTransponder(quint64 id,
                      SAKUdpClientParametersContext parasCtx,
                      QWidget *parent = Q_NULLPTR);
    ~SAKUdpTransponder();
    QVariant parametersContext() final;
protected:
    SAKDebuggerDevice *device() final;
    void onDeviceStateChanged(bool opened) final;
private:
    Ui::SAKUdpTransponder *mUi;
    SAKUdpClientDevice *mDevice;
private:
    void initSignals();
};

#endif // SAKUDPTRANSPONDER_HH
