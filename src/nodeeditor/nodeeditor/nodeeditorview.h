/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModelRegistry>

#include <QFrame>
#include <QMap>
#include <QPoint>
#include <QPointF>
#include <QVariant>
#include <QVariantList>

class NodeEditorScene;
class NodeEditorRuler;
class NodeEditorView : public QtNodes::GraphicsView
{
    Q_OBJECT
public:
    enum AlignType {
        AlignTypeTop,
        AlignTypeBottom,
        AlignTypeVCenter,
        AlignTypeLeft,
        AlignTypeRight,
        AlignTypeHCenter,
        AlignTypeHEquidistance,
        AlignTypeVEquidistance
    };

public:
    explicit NodeEditorView(const QColor &rulerColor, QWidget *parent = nullptr);

    void saveProject(const QString &fileName);
    void loadProject();
    void loadProject(const QString &fileName);
    int nodesCount();
    void deleteNode(QtNodes::NodeId nodeId);
    void alignNodes(int alignType);
    void fitInViewToAllItems();
    void deleteSelectedNodes();
    void clearAllNodes();
    void selectAllNodes();
    void setStylesheet(const QJsonObject &style);
    void setGridVisible(bool visible);

    static QRectF defaultSceneRect();
    QtNodes::DataFlowGraphModel *model();

signals:
    void sceneLoaded();
    void connectionCreated();
    void connectionDeleted();
    void nodeCreated(QtNodes::NodeId const nodeId);
    void nodeDeleted(QtNodes::NodeId const nodeId);
    void nodeUpdated();
    void nodeFlagsUpdated();
    void nodePositionUpdated();
    void modelReset();
    void nodeClicked(QtNodes::NodeId const nodeId);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &r) override;
    void paintEvent(QPaintEvent *event) override;

private:
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> m_registry;
    QtNodes::DataFlowGraphModel *m_model;
    QtNodes::NodeId m_latestSelectedNodeId{QtNodes::InvalidNodeId};

    NodeEditorScene *m_scene;
    QPointF m_clickPos;
    bool m_gridVisible{true};
    QRect m_rubberBandRect;

private:
    void alignNodesToTop();
    void alignNodesToBottom();
    void alignNodesToLeft();
    void alignNodesToRight();
    void alignNodesToHCenter();
    void alignNodesToVCenter();
    void alignNodesToHEquidistance();
    void alignNodesToVEquidistance();

    void onRubberBandChanged(QRect viewportRect, QPointF fromScenePoint, QPointF toScenePoint);

    qreal minXOfSelectedNodes();
    qreal minYOfSelectedNodes();
    qreal maxXOfSelectedNodes();
    qreal maxYOfSelectedNodes();

    QList<QtNodes::NodeId> orderSelectedNodesByX();
    QList<QtNodes::NodeId> orderSelectedNodesByY();
    QPixmap toPixMap(QGraphicsItem *item);
};
