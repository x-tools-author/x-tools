/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEDEVICEMASK_HH
#define SAKDEBUGPAGEDEVICEMASK_HH

#include <QWidget>

namespace  Ui{
    class SAKDebugPageDeviceMask;
}

class SAKDebugPage;
class SAKDebugPageDeviceMask : public QWidget
{
    Q_OBJECT
public:
    explicit SAKDebugPageDeviceMask(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageDeviceMask();
private:
    SAKDebugPage *mDebugPage;
private:
    Ui::SAKDebugPageDeviceMask *ui;
};

#endif // SAKDEBUGPAGEDEVICEMASK_HH
