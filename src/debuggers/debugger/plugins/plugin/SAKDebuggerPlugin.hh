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

class SAKDebuggerPlugin : public QObject
{
    Q_OBJECT
public:
    struct SAKStructDebuggerPluginContext {
        int majorVersion;
        int minorVersion;
        int patchVersion;

        QString author;
        QString email;
        QString qqNumber;
        QString copyright;
        QString description;

        QString giteeRepositories;
        QString githubRepositories;
        QString gitlabRepositories;
        QString codingRepositories;
    };


public:
    SAKDebuggerPlugin(QObject *parent = Q_NULLPTR);
    ~SAKDebuggerPlugin();
    const QString qtVersion();

    /**
     * @brief pluginPanel: Get the plugin ui.
     * @return The plugin ui.
     */
    virtual QWidget* pluginPanel() = 0;

    /**
     * @brief bytesRead: After device reading bytes, application will called the
     * interface. You can do what you want in the function.
     * @param bytes: Bytes read.
     */
    virtual void bytesRead(QByteArray bytes) = 0;

    /**
     * @brief pluginContext
     * @return
     */
    virtual SAKStructDebuggerPluginContext pluginContext() = 0;
signals:
    /**
     * @brief writeRawBytes: The application will connect the signal automatically.
     * The cookedBytes will be write to device after cooking by using input parameters.
     * @param rawBytes: Bytes need to be write.
     */
    void writeRawBytes(QByteArray rawBytes);

    /**
     * @brief writeRawBytes: The application will connect the signal automatically.
     * The cookedBytes will be write to device directly.
     * @param cookedBytes: Bytes need to be write.
     */
    void writeCookedBytes(QByteArray cookedBytes);
};

QT_BEGIN_NAMESPACE
#define SAKDebuggerPlugin_iid "QSAK.SAKDebuggerPlugin"
Q_DECLARE_INTERFACE(SAKDebuggerPlugin, SAKDebuggerPlugin_iid)
QT_END_NAMESPACE

#endif // SAKDEBUGGERPLUGIN_HH
