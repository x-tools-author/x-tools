/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERCONTROLLER_H
#define SAKDEBUGGERCONTROLLER_H

#include <QWidget>

class SAKDebugger;
/// @brief Device controller
class SAKDebuggerController:public QWidget
{
    Q_OBJECT
public:
    SAKDebuggerController(QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerController();

    /**
     * @brief setUiEnable: Set ui state
     * @param opened: If the device is opened, the value of the parameters is true
     */
    virtual void updateUiState(bool opened) = 0;

    /**
     * @brief refreshDevice: Refresh the device list
     */
    virtual void refreshDevice() = 0;
signals:
    void messageChanged(QString msg, bool isError = true);
    void parametersChanged();
};

#endif
