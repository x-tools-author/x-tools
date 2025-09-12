/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModel>

#include <QJsonObject>
#include <QThread>
#include <QWidget>

struct AbstractPluginParametersKeys
{
    QString pluginId{"plugin-id"};
    QString pluginName{"plugin-name"};
};
struct AbstractPluginParameters
{
    QString pluginId;
    QString pluginName;
};

QJsonObject abstractPluginParametersToJsonObject(const AbstractPluginParameters &parameters);
AbstractPluginParameters abstractPluginParametersFromJsonObject(const QJsonObject &jsonObj);

class AbstractPlugin : public QThread
{
    Q_OBJECT
public:
    explicit AbstractPlugin(const QString &libFile, QObject *parent = nullptr);
    virtual ~AbstractPlugin() override;
    AbstractPlugin(const AbstractPlugin &) = delete;
    AbstractPlugin &operator=(const AbstractPlugin &) = delete;

    static void setLanguage(const QString &language);

    void inputBytes(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const index);
signals:
    void outputBytes(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const index);

public:
    virtual QString caption() const;
    virtual QString name() const;
    virtual unsigned int inPorts() const;
    virtual unsigned int outPorts() const;
    virtual QWidget *embeddedWidget();

    virtual QByteArray handleData(QByteArray const &data, int const index);
    virtual QJsonObject save() const;
    virtual void load(const QJsonObject &parameters);

public:
    void run() override;

protected:
    QString m_libFile;
    static QString m_language;

private:
    Q_SIGNAL void input2thread(std::shared_ptr<QtNodes::NodeData> nodeData,
                               QtNodes::PortIndex const index);
};
