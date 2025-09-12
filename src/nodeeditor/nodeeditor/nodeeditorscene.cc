/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditorscene.h"

#include <QtNodes/internal/AbstractGraphModel.hpp>
#include <QtNodes/internal/BasicGraphicsScene.hpp>
#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>

#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QHeaderView>
#include <QJsonArray>
#include <QLineEdit>
#include <QMimeData>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUndoStack>
#include <QVBoxLayout>
#include <QWidgetAction>

#if defined(X_MS_STORE)
#include "microsoftstore.h"
#include <QMessageBox>
#endif

#include "nodeeditorregistry.h"

static void insertSerializedItems(QJsonObject const &json, QtNodes::BasicGraphicsScene *scene)
{
    QtNodes::AbstractGraphModel &graphModel = scene->graphModel();

    QJsonArray const &nodesJsonArray = json["nodes"].toArray();

    for (QJsonValue &&node : nodesJsonArray) {
        QJsonObject obj = node.toObject();

        graphModel.loadNode(obj);

        auto id = obj["id"].toInt();
        scene->nodeGraphicsObject(id)->setZValue(1.0);
        scene->nodeGraphicsObject(id)->setSelected(true);
    }

    QJsonArray const &connJsonArray = json["connections"].toArray();

    for (QJsonValue &&connection : connJsonArray) {
        QJsonObject connJson = connection.toObject();

        QtNodes::ConnectionId connId = QtNodes::fromJson(connJson);

        // Restore the connection
        graphModel.addConnection(connId);

        scene->connectionGraphicsObject(connId)->setSelected(true);
    }
}

class CreateCommand : public QUndoCommand
{
public:
    CreateCommand(QtNodes::BasicGraphicsScene *scene,
                  QString const name,
                  QPointF const &mouseScenePos);

    void undo() override;
    void redo() override;

private:
    QtNodes::BasicGraphicsScene *_scene;
    QtNodes::NodeId _nodeId;
    QJsonObject _sceneJson;
};

CreateCommand::CreateCommand(QtNodes::BasicGraphicsScene *scene,
                             QString const name,
                             QPointF const &mouseScenePos)
    : _scene(scene)
    , _sceneJson(QJsonObject())
{
    _nodeId = _scene->graphModel().addNode(name);
    if (_nodeId != QtNodes::InvalidNodeId) {
        _scene->graphModel().setNodeData(_nodeId, QtNodes::NodeRole::Position, mouseScenePos);
    } else {
        setObsolete(true);
    }
}

void CreateCommand::undo()
{
    QJsonArray nodesJsonArray;
    nodesJsonArray.append(_scene->graphModel().saveNode(_nodeId));
    _sceneJson["nodes"] = nodesJsonArray;

    _scene->graphModel().deleteNode(_nodeId);
}

void CreateCommand::redo()
{
    if (_sceneJson.empty() || _sceneJson["nodes"].toArray().empty())
        return;

    insertSerializedItems(_sceneJson, _scene);
}

NodeEditorScene::NodeEditorScene(QtNodes::DataFlowGraphModel &graphModel, QObject *parent)
    : QtNodes::DataFlowGraphicsScene{graphModel, parent}
    , m_graphModel{graphModel}
{}

QMenu *NodeEditorScene::createSceneMenu(QPointF const scenePos)
{
    QMenu *modelMenu = new QMenu();

    // Add filterbox to the context menu
    auto *txtBox = new QLineEdit(modelMenu);
    txtBox->setPlaceholderText(QStringLiteral("Filter"));
    txtBox->setClearButtonEnabled(true);

    // Add result tree view to the context menu
    QTreeWidget *treeView = new QTreeWidget(modelMenu);
    treeView->header()->close();
    treeView->setMinimumHeight(400);

    QWidget *tmpWidget = new QWidget(modelMenu);
    QVBoxLayout *layout = new QVBoxLayout(tmpWidget);
    layout->addWidget(txtBox);
    layout->addWidget(treeView);
    layout->setContentsMargins(0, 4, 0, 0);
    tmpWidget->adjustSize();
    tmpWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    auto *widgetAction = new QWidgetAction(modelMenu);
    widgetAction->setDefaultWidget(tmpWidget);
    modelMenu->addAction(widgetAction);

    auto registry = m_graphModel.dataModelRegistry();
    std::shared_ptr<NodeEditorRegistry> xRegistry = std::static_pointer_cast<NodeEditorRegistry>(
        registry);

    for (auto &category : xRegistry->categoriesOrdered()) {
        auto item = new QTreeWidgetItem(treeView);
        auto friendlyCategory = xRegistry->friendlyNodeCategory(category);
        item->setText(0, friendlyCategory);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        for (auto &ctx : xRegistry->categoryNodeContexts(category)) {
            auto nodeItem = new QTreeWidgetItem(item);
            nodeItem->setText(0, ctx.caption);
            nodeItem->setData(0, Qt::UserRole, ctx.name);
        }
    }
#if 0
    for (auto const &assoc : registry->registeredModelsCategoryAssociation()) {
        QList<QTreeWidgetItem *> parent = treeView->findItems(assoc.second, Qt::MatchExactly);

        if (parent.count() <= 0)
            continue;

        auto item = new QTreeWidgetItem(parent.first());
        const QString name = xRegistry->friendlyNodeName(assoc.first);
        item->setText(0, name);
        item->setData(0, Qt::UserRole, assoc.first);
    }
#endif

    treeView->expandAll();

    connect(treeView,
            &QTreeWidget::itemClicked,
            this,
            [this, modelMenu, scenePos](QTreeWidgetItem *item, int) {
                if (!(item->flags() & (Qt::ItemIsSelectable))) {
                    return;
                }

                const QString name = item->data(0, Qt::UserRole).toString();
                this->undoStack().push(new CreateCommand(this, name, scenePos));

                modelMenu->close();
            });

    //Setup filtering
    connect(txtBox, &QLineEdit::textChanged, this, [treeView](const QString &text) {
        QTreeWidgetItemIterator categoryIt(treeView, QTreeWidgetItemIterator::HasChildren);
        while (*categoryIt)
            (*categoryIt++)->setHidden(true);
        QTreeWidgetItemIterator it(treeView, QTreeWidgetItemIterator::NoChildren);
        while (*it) {
            auto modelName = (*it)->text(0);
            const bool match = (modelName.contains(text, Qt::CaseInsensitive));
            (*it)->setHidden(!match);
            if (match) {
                QTreeWidgetItem *parent = (*it)->parent();
                while (parent) {
                    parent->setHidden(false);
                    parent = parent->parent();
                }
            }
            ++it;
        }
    });

    // make sure the text box gets focus so the user doesn't have to click on it
    txtBox->setFocus();

    // QMenu's instance auto-destruction
    modelMenu->setAttribute(Qt::WA_DeleteOnClose);
    treeView->resize(treeView->sizeHint());
    return modelMenu;
}

QtNodes::DataFlowGraphModel &NodeEditorScene::dataFlowGraphModel()
{
    return m_graphModel;
}

void NodeEditorScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    DataFlowGraphicsScene::dragEnterEvent(event);
    event->accept();
}

void NodeEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    DataFlowGraphicsScene::dragEnterEvent(event);
    event->accept();
}

void NodeEditorScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    DataFlowGraphicsScene::dragLeaveEvent(event);
    event->accept();
}

void NodeEditorScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData) {
        int row, col;
        QMap<int, QVariant> roleDataMap;
        QDataStream stream(mimeData->data("application/x-qabstractitemmodeldatalist"));
        while (!stream.atEnd()) {
            // https://cloud.tencent.com/developer/ask/sof/102669218
            stream >> row >> col >> roleDataMap;
        }

        QString name = roleDataMap.value(Qt::UserRole).toString();
        qInfo() << "Node name:" << name;
        m_nodeId = graphModel().addNode(name);
        graphModel().setNodeData(m_nodeId, QtNodes::NodeRole::Position, event->scenePos());
        nodeGeometry().recomputeSize(m_nodeId);
    }

    DataFlowGraphicsScene::dropEvent(event);
    event->accept();
}
