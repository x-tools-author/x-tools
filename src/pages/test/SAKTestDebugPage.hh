/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTESTDEBUGPAGE_HH
#define SAKTESTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKTestDeviceController;
// The debugging page is used by developer only.
class SAKTestDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKTestDebugPage(QWidget *parent = Q_NULLPTR);

    SAKDebugPageController *deviceController() final;
    SAKDebugPageDevice *createDevice() final;
private:
    SAKTestDeviceController *mDeviceController;
};

#endif
