/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGIN_HH
#define SAKDEBUGGERPLUGIN_HH

#include <QWidget>

class PluginUi : public QWidget {
    Q_OBJECT
signals:
    void onDataRead(const QByteArray &data);
    void onDataWritten(const QByteArray &data);
signals:
    void invokeWriteRawData(const QString &data);
    void invokeWriteCookedData(const QByteArray &data);
};

class SAKDebuggerPlugin : public QObject
{
    Q_OBJECT
public:
    SAKDebuggerPlugin(QObject *parent = Q_NULLPTR){Q_UNUSED(parent)};
    virtual PluginUi *ui() = 0;
};

QT_BEGIN_NAMESPACE
#define SAKDebuggerPluginIID "QSAK.Plugin.SAKDebuggerPlugin"
Q_DECLARE_INTERFACE(SAKDebuggerPlugin, SAKDebuggerPluginIID)
QT_END_NAMESPACE

#endif // SAKDEBUGGERPLUGIN_HH
