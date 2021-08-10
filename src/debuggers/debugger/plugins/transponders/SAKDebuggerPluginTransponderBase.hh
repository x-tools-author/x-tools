/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINTRANSPONDERBASE_HH
#define SAKDEBUGGERPLUGINTRANSPONDERBASE_HH

#include <QWidget>

class SAKDebuggerPluginTransponderBase : public QWidget
{
    Q_OBJECT
public:
    SAKDebuggerPluginTransponderBase(QWidget *parent);
    ~SAKDebuggerPluginTransponderBase();

    void setId(quint64 id){mId = id;}
    quint64 id(){return mId;}


private:
    quint64 mId;
};

#endif
