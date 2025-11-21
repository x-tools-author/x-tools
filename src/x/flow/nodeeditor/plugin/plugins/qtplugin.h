/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QWidget>

#include "abstractplugin.h"

class XFlowPluginInterfaceV1
{
public:
    virtual QString pluginApiVersion() const = 0;

    virtual QString caption() const = 0;
    virtual QString name() const = 0;
    virtual int inPorts() const = 0;
    virtual int outPorts() const = 0;
    virtual QWidget *newWidget() = 0;
    virtual QByteArray handleData(QByteArray const &data, int const index, QWidget *ui) = 0;

    virtual QJsonObject save(QWidget *ui) const { return QJsonObject(); }
    virtual void load(const QJsonObject &parameters, QWidget *ui) { Q_UNUSED(parameters); }
    virtual void setLanguage(const QString &language) { Q_UNUSED(language); }
};

QT_BEGIN_NAMESPACE
#define XFlowPluginInterfaceV1_iid "xtools.xflow.XFlowPluginInterfaceV1"
Q_DECLARE_INTERFACE(XFlowPluginInterfaceV1, XFlowPluginInterfaceV1_iid)
QT_END_NAMESPACE

class QtPlugin : public AbstractPlugin
{
    Q_OBJECT
public:
    explicit QtPlugin(const QString &libFile, QObject *parent = nullptr);
    ~QtPlugin() override;
    QtPlugin(const QtPlugin &) = delete;
    QtPlugin &operator=(const QtPlugin &) = delete;

    static bool testLib(const QString &file);

    QString caption() const override;
    QString name() const override;
    unsigned int inPorts() const override;
    unsigned int outPorts() const override;
    QWidget *embeddedWidget() override;

    QByteArray handleData(QByteArray const &data, int const index) override;
    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    XFlowPluginInterfaceV1 *m_plugin{nullptr};
    QWidget *m_embeddedWidget{nullptr};
};
