/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/NodeData>

#define X_TRIAL_NODES_COUNT 8

class NodeEditorScene : public QtNodes::DataFlowGraphicsScene
{
    Q_OBJECT
public:
    explicit NodeEditorScene(QtNodes::DataFlowGraphModel &graphModel, QObject *parent = nullptr);

    QMenu *createSceneMenu(QPointF const scenePos) override;
    QtNodes::DataFlowGraphModel &dataFlowGraphModel();

protected:
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    QtNodes::NodeId m_nodeId;
    QtNodes::DataFlowGraphModel &m_graphModel;
};
