/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGECONTROLLER_HH
#define SAKDEBUGPAGECONTROLLER_HH

#include <QWidget>

class SAKDebugPage;
/// @brief Device controller
class SAKDebugPageController:public QWidget
{
    Q_OBJECT
public:
    SAKDebugPageController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageController();

    /**
     * @brief parameters: Get the parameters of the device controller,
     * the specific type is determined by the subclass
     * @return The parameters of the device controller
     */
    virtual QVariant parameters();

    /**
     * @brief setUiEnable: Set ui state
     * @param opened: If the device is opened, the value of the parameters is true
     */
    virtual void setUiEnable(bool opened);

    /**
     * @brief refreshDevice: Refresh the device list
     */
    virtual void refreshDevice();
protected:
    SAKDebugPage *mDebugPage;
signals:
    void messageChange(QString msg, bool isInfo);
};

#endif
