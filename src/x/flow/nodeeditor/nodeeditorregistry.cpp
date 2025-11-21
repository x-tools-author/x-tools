/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditorregistry.h"

#include "nodes/communication/communication.h"
#include "nodes/hub/hub.h"

#include "nodes/inputoutput/datapersistence.h"
#include "nodes/inputoutput/dataview.h"
#include "nodes/inputoutput/debugoutput.h"
#include "nodes/inputoutput/randomgenerator.h"
#include "nodes/inputoutput/userinput.h"

#include "nodes/script/commandline.h"
#include "nodes/script/javascript.h"
#include "nodes/script/luascript.h"

#include "nodes/dataprocess/aescipher.h"
#include "nodes/dataprocess/crccalculator.h"
#include "nodes/dataprocess/crcchecker.h"
#include "nodes/dataprocess/filter.h"
#include "nodes/dataprocess/frameanalyzer.h"
#include "nodes/dataprocess/masker.h"
#include "nodes/dataprocess/sumcalculator.h"
#include "nodes/dataprocess/sumchecker.h"

#include "nodes/instruments/flowmeter.h"

#include "device/devicemanager.h"
#include "plugin/pluginmanager.h"
#include "plugin/pluginnode.h"

namespace xFlow {

template<typename T>
void registerNode(NodeEditorRegistry *nodeRegister,
                  const QString &category,
                  const QString &friendlyCategory)
{
    nodeRegister->registerModel<T>([]() { return std::make_unique<T>(); }, category);

    auto node = new T();
    NodeEditorRegistry::NodeContext nodeContext;
    nodeContext.category = category;
    nodeContext.friendlyCategory = friendlyCategory;
    nodeContext.caption = node->caption();
    nodeContext.name = node->name();
    nodeRegister->addNodeContext(nodeContext);
    node->deleteLater();
}

NodeEditorRegistry::NodeEditorRegistry()
{
    registerModelCommunication();
    registerModelInputOutput();
    registerModelDataProcess();
    registerModelInstruments();
    registerModelScript();
    registerModelHub();
    registerModelPlugin();
}

void NodeEditorRegistry::registerModelCommunication()
{
    static const QString category = QString("Communication");
    static const QString friendlyCategory = QObject::tr("Communication");
#if 0
    QList<int> deviceTypes = DeviceFactory::singleton().supportedDeviceTypes();
    for (int &type : deviceTypes) {
        // clang-format off
        registerModel<Communication>([=]() { return std::make_unique<Communication>(type); }, category);
        // clang-format on

        auto node = new Communication(type);
        NodeEditorRegistry::NodeContext nodeContext;
        nodeContext.category = category;
        nodeContext.friendlyCategory = friendlyCategory;
        nodeContext.caption = node->caption();
        nodeContext.name = node->name();
        addNodeContext(nodeContext);
    }
#endif
}

template<typename T>
void registerModelInputOutput(NodeEditorRegistry *nodeRegister,
                              const QString &category,
                              const QString &friendlyCategory)
{
    nodeRegister->registerModel<T>([]() { return std::make_unique<T>(); }, category);

    auto model = new T();
    NodeEditorRegistry::NodeContext nodeContext;
    nodeContext.category = category;
    nodeContext.friendlyCategory = friendlyCategory;
    nodeContext.caption = model->caption();
    nodeContext.name = model->name();
    nodeRegister->addNodeContext(nodeContext);
    model->deleteLater();
}

void NodeEditorRegistry::registerModelInputOutput()
{
    static const QString category = QString("Input/Output");
    static const QString friendlyCategory = QObject::tr("Input/Output");
    xFlow::registerModelInputOutput<UserInput>(this, category, friendlyCategory);
    xFlow::registerModelInputOutput<DebugOutput>(this, category, friendlyCategory);
    xFlow::registerModelInputOutput<RandomGenerator>(this, category, friendlyCategory);
    xFlow::registerModelInputOutput<DataPersistence>(this, category, friendlyCategory);
    xFlow::registerModelInputOutput<DataView>(this, category, friendlyCategory);
}

void NodeEditorRegistry::registerModelDataProcess()
{
    static const QString category = QString("DataProcess");
    static const QString friendlyCategory = QObject::tr("Data Process");

    registerNode<AesCipher>(this, category, friendlyCategory);
    registerNode<CrcCalculator>(this, category, friendlyCategory);
    registerNode<CrcChecker>(this, category, friendlyCategory);
    registerNode<Filter>(this, category, friendlyCategory);
    registerNode<FrameAnalyzer>(this, category, friendlyCategory);
    registerNode<Masker>(this, category, friendlyCategory);
    registerNode<SumCalculator>(this, category, friendlyCategory);
    registerNode<SumChecker>(this, category, friendlyCategory);
}

void NodeEditorRegistry::registerModelInstruments()
{
    static const QString category = QString("Instruments");
    static const QString friendlyCategory = QObject::tr("Instruments");

    registerNode<Flowmeter>(this, category, friendlyCategory);
}

void NodeEditorRegistry::registerModelScript()
{
    static const QString category = QString("Scripts");
    static const QString friendlyCategory = QObject::tr("Scripts");

    registerNode<CommandLine>(this, category, friendlyCategory);
    registerNode<JavaScript>(this, category, friendlyCategory);
    registerNode<LuaScript>(this, category, friendlyCategory);
}

void NodeEditorRegistry::registerModelHub()
{
    static const QString category = QString("Hub");
    static const QString friendlyCategory = QObject::tr("Hub");

    struct Ctx
    {
        int inPorts;
        int outPorts;
    };

    QList<Ctx> list;
    list << Ctx{1, 2} << Ctx{1, 4} << Ctx{1, 8};
    list << Ctx{2, 1} << Ctx{4, 1} << Ctx{8, 1};
    for (Ctx &ctx : list) {
        // clang-format off
        registerModel<Hub>([=]() { return std::make_unique<Hub>(ctx.inPorts, ctx.outPorts); }, category);
        // clang-format on

        Hub *hub = new Hub(ctx.inPorts, ctx.outPorts);
        NodeEditorRegistry::NodeContext nodeContext;
        nodeContext.category = category;
        nodeContext.friendlyCategory = friendlyCategory;
        nodeContext.caption = hub->caption();
        nodeContext.name = hub->name();
        addNodeContext(nodeContext);
        delete hub;
    }
}

void NodeEditorRegistry::registerModelPlugin()
{
    static const QString category = QString("Plugins");
    static const QString friendlyCategory = QObject::tr("Plugins");

    PluginManager &mgr = PluginManager::singleton();
    QList<PluginManager::PluginContext> pluginCtxs = mgr.plugins();

    for (PluginManager::PluginContext &pluginCtx : pluginCtxs) {
        // clang-format off
        registerModel<PluginNode>([=]() { return std::make_unique<PluginNode>(pluginCtx.libFile); }, category);
        // clang-format on

        NodeContext ctx;
        ctx.category = category;
        ctx.friendlyCategory = friendlyCategory;
        ctx.caption = pluginCtx.caption;
        ctx.name = pluginCtx.name;
        addNodeContext(ctx);
    }
}

QStringList NodeEditorRegistry::categoriesOrdered() const
{
    QStringList categories;
    for (const NodeContext &context : m_nodeContexts) {
        if (categories.contains(context.category)) {
            continue;
        }

        categories.append(context.category);
    }

    return categories;
}

QList<NodeEditorRegistry::NodeContext> NodeEditorRegistry::categoryNodeContexts(
    const QString &category) const
{
    QList<NodeContext> nodeContexts;
    for (const NodeContext &context : m_nodeContexts) {
        if (context.category == category) {
            nodeContexts.append(context);
        }
    }

    return nodeContexts;
}

void NodeEditorRegistry::addNodeContext(const NodeContext &nodeContext)
{
    m_nodeContexts.append(nodeContext);
}

QString NodeEditorRegistry::friendlyNodeName(const QString &name) const
{
    for (const NodeContext &context : m_nodeContexts) {
        if (context.name == name) {
            return context.caption;
        }
    }

    return name;
}

QString NodeEditorRegistry::friendlyNodeCategory(const QString &category) const
{
    for (const NodeContext &context : m_nodeContexts) {
        if (context.category == category) {
            return context.friendlyCategory;
        }
    }

    return category;
}

} // namespace xFlow