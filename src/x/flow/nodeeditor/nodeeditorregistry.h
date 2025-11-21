/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/NodeDelegateModelRegistry>

class NodeEditorRegistry : public QtNodes::NodeDelegateModelRegistry
{
public:
    struct NodeContext
    {
        QString category;
        QString friendlyCategory;
        QString caption;
        QString name;
    };

public:
    NodeEditorRegistry();
    ~NodeEditorRegistry() = default;

    void registerModelCommunication();
    void registerModelInputOutput();
    void registerModelDataProcess();
    void registerModelInstruments();
    void registerModelHub();
    void registerModelScript();
    void registerModelPlugin();

    QStringList categoriesOrdered() const;
    QList<NodeContext> categoryNodeContexts(const QString &category) const;
    void addNodeContext(const NodeContext &nodeContext);
    QString friendlyNodeName(const QString &name) const;
    QString friendlyNodeCategory(const QString &category) const;

private:
    QList<NodeContext> m_nodeContexts;
};
