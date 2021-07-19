/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKPLUGINDATAFORWARDING_HH
#define SAKPLUGINDATAFORWARDING_HH

#include <QWidget>

namespace Ui {
    class SAKPluginDataForwarding;
}

class SAKPluginDataForwarding:public QWidget
{
    Q_OBJECT
public:
    SAKPluginDataForwarding(QWidget *parent = Q_NULLPTR);
    ~SAKPluginDataForwarding();
private:
    Ui::SAKPluginDataForwarding *mUi;
};

#endif
