/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditorview.h"

#include <QtNodes/NodeDelegateModelRegistry>
#include <QtNodes/internal/AbstractNodePainter.hpp>
#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include <QtNodes/internal/NodeGraphicsObject.hpp>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QMouseEvent>
#include <QPalette>
#include <QPixmap>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QtOpenGLWidgets/QtOpenGLWidgets>

#include "application.h"
#include "nodeeditorregistry.h"
#include "nodeeditorscene.h"
#include "nodes/common/basenodeui.h"

namespace xFlow {

NodeEditorView::NodeEditorView(const QColor &rulerColor, QWidget *parent)
    : QtNodes::GraphicsView{parent}
{
#if 0
    setViewport(new QOpenGLWidget);
#endif
    m_registry = std::make_shared<NodeEditorRegistry>();
    m_model = new QtNodes::DataFlowGraphModel(m_registry);
    m_scene = new NodeEditorScene(*m_model, this);
    setScene(m_scene);
    setObjectName("NodeEditorView");
    setStyleSheet("QGraphicsView#NodeEditorView { border: none; }");

    // clang-format off
    connect(m_scene, &QtNodes::DataFlowGraphicsScene::sceneLoaded, this, &NodeEditorView::sceneLoaded);
    connect(m_scene, &QtNodes::DataFlowGraphicsScene::nodeClicked, this, &NodeEditorView::nodeClicked);
    connect(m_scene, &QtNodes::DataFlowGraphicsScene::nodeSelected, this, [this](const QtNodes::NodeId &id) { this->m_latestSelectedNodeId = id; });
#if 1
    connect(m_scene, &QtNodes::DataFlowGraphicsScene::nodeDoubleClicked, this, &NodeEditorView::onNodeDoubleClicked);
#endif
    connect(m_model, &QtNodes::DataFlowGraphModel::connectionCreated, this, &NodeEditorView::connectionCreated);
    connect(m_model, &QtNodes::DataFlowGraphModel::connectionDeleted, this, &NodeEditorView::connectionDeleted);
    connect(m_model, &QtNodes::DataFlowGraphModel::nodeCreated, this, &NodeEditorView::onNodeCreated);
    connect(m_model, &QtNodes::DataFlowGraphModel::nodeDeleted, this, &NodeEditorView::nodeDeleted);
    connect(m_model, &QtNodes::DataFlowGraphModel::nodeUpdated, this, &NodeEditorView::nodeUpdated);
    connect(m_model, &QtNodes::DataFlowGraphModel::nodeFlagsUpdated, this, &NodeEditorView::nodeFlagsUpdated);
    connect(m_model, &QtNodes::DataFlowGraphModel::nodePositionUpdated, this, &NodeEditorView::nodePositionUpdated);
    connect(m_model, &QtNodes::DataFlowGraphModel::modelReset, this, &NodeEditorView::modelReset);
    // clang-format on

    setDragMode(RubberBandDrag);
    m_scene->setSceneRect(defaultSceneRect());
    setFixedSize(m_scene->sceneRect().size().toSize());
    fitInView(m_scene->sceneRect());
    setContentsMargins(0, 0, 0, 0);
    viewport()->setContentsMargins(0, 0, 0, 0);

    connect(this, &NodeEditorView::rubberBandChanged, this, &NodeEditorView::onRubberBandChanged);
}

void NodeEditorView::saveProject(const QString &fileName)
{
    if (fileName.isEmpty()) {
        auto scene = static_cast<NodeEditorScene *>(this->scene());
        scene->save();
    } else {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
            QJsonObject obj = m_model->save();
            QJsonDocument doc;
            doc.setObject(obj);
            QTextStream out(&file);
            out << doc.toJson();
            file.close();
        } else {
            qWarning() << QString("Save file(%1) failed: %2").arg(fileName, file.errorString());
        }
    }
}

void NodeEditorView::loadProject()
{
    auto scene = static_cast<NodeEditorScene *>(this->scene());
    scene->load();
}

void NodeEditorView::loadProject(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        clearAllNodes();
        QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();

        try {
            m_model->load(obj);
        } catch (const std::logic_error &e) {
            qWarning("Load node(%s) failed!", e.what());
        }
    } else {
        qWarning() << "Can not open project file:" << fileName;
    }
}

int NodeEditorView::nodesCount()
{
    return m_model->allNodeIds().size();
}

void NodeEditorView::deleteNode(QtNodes::NodeId nodeId)
{
    m_model->deleteNode(nodeId);
}

void NodeEditorView::alignNodes(int alignType)
{
    switch (alignType) {
    case NodeEditorView::AlignTypeTop:
        alignNodesToTop();
        break;
    case NodeEditorView::AlignTypeBottom:
        alignNodesToBottom();
        break;
    case NodeEditorView::AlignTypeLeft:
        alignNodesToLeft();
        break;
    case NodeEditorView::AlignTypeRight:
        alignNodesToRight();
        break;
    case NodeEditorView::AlignTypeHCenter:
        alignNodesToHCenter();
        break;
    case NodeEditorView::AlignTypeVCenter:
        alignNodesToVCenter();
        break;
    case NodeEditorView::AlignTypeHEquidistance:
        alignNodesToHEquidistance();
        break;
    case NodeEditorView::AlignTypeVEquidistance:
        alignNodesToVEquidistance();
        break;
    default:
        break;
    }
}

void NodeEditorView::fitInViewToAllItems()
{
    QRectF rectF = m_scene->itemsBoundingRect();
    fitInView(rectF, Qt::KeepAspectRatio);
}

void NodeEditorView::deleteSelectedNodes()
{
    auto nodeIds = m_scene->selectedNodes();
    for (QtNodes::NodeId id : nodeIds) {
        m_model->deleteNode(id);
    }
}

void NodeEditorView::clearAllNodes()
{
    m_scene->clearScene();
}

void NodeEditorView::selectAllNodes()
{
    auto items = m_scene->items();
    for (auto &item : items) {
        item->setSelected(true);
    }
}

void NodeEditorView::setStylesheet(const QJsonObject &style)
{
    QString tmp = QString::fromLatin1(QJsonDocument(style).toJson());
    QtNodes::StyleCollection::setNodeStyle(tmp);

    tmp = QString::fromLatin1(QJsonDocument(style).toJson());
    QtNodes::StyleCollection::setConnectionStyle(tmp);

    tmp = QString::fromLatin1(QJsonDocument(style).toJson());
    QtNodes::StyleCollection::setGraphicsViewStyle(tmp);

    QJsonObject graphicsViewStyle = style["GraphicsViewStyle"].toObject();
    QJsonArray bgColor = graphicsViewStyle["BackgroundColor"].toArray();
    QColor cookedBgColor(bgColor.at(0).toInt(), bgColor.at(1).toInt(), bgColor.at(2).toInt());
    setBackgroundBrush(cookedBgColor);
}

void NodeEditorView::setGridVisible(bool visible)
{
    m_gridVisible = visible;
}

QRectF NodeEditorView::defaultSceneRect()
{
    static QRectF rect(1, 1, 3840, 2160);
    return rect;
}

QtNodes::DataFlowGraphModel *NodeEditorView::model()
{
    return m_model;
}

NodeEditorScene *NodeEditorView::cookedScene()
{
    return m_scene;
}

void NodeEditorView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!itemAt(event->pos())) {
        GraphicsView::contextMenuEvent(event);
        return;
    }

    auto *item = itemAt(event->pos());
    auto connection = dynamic_cast<QtNodes::ConnectionGraphicsObject *>(item);
    if (connection) {
        QMenu *menu = new QMenu(this);
        menu->addAction(tr("Delete connection"), this, [=]() {
            m_model->deleteConnection(connection->connectionId());
        });
        menu->exec(event->globalPos());
        return;
    }

    auto node = dynamic_cast<QtNodes::NodeGraphicsObject *>(item);
    if (!node) {
        // 如果节点嵌入了widget, 点击时鼠标位置可能在widget上
        QGraphicsProxyWidget *w = dynamic_cast<QGraphicsProxyWidget *>(item);
        if (!w) {
            return;
        }

        // 鼠标位置在widget上，需要找到widget所在的节点（parentItem）
        item = item->parentItem();
        if (!item) {
            return;
        }

        node = dynamic_cast<QtNodes::NodeGraphicsObject *>(item);
        if (!node) {
            return;
        }
    }

    QMenu *menu = new QMenu(this);
#if 1
    menu->addAction(tr("Save Image as..."), this, [=]() {
        auto tmp = m_saveImagePath.isEmpty()
                       ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                       : m_saveImagePath;
        tmp += "/" + m_model->nodeData(node->nodeId(), QtNodes::NodeRole::Caption).toString();
        tmp += ".png";
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Image"),
                                                        tmp,
                                                        tr("Images (*.png)"));
        if (fileName.isEmpty()) {
            return;
        }

        QString path = fileName.left(fileName.lastIndexOf('/'));
        m_saveImagePath = path;

        QPixmap pixMap = toPixMap(item);
        pixMap.save(fileName);
    });
#endif
    menu->addAction(tr("Save Image to Clipboard"), this, [=]() {
        QApplication::clipboard()->setPixmap(toPixMap(item));
    });
    menu->addAction(tr("Delete"), this, [=]() {
#if 0
        // 删除所有选中的节点
        auto nodeIds = m_scene->selectedNodes();
        for (QtNodes::NodeId &id : nodeIds) {
            m_model->deleteNode(id);
        }
#else
        // 删除当前节点
        m_model->deleteNode(node->nodeId());
#endif
    });

    menu->exec(event->globalPos());
    menu->deleteLater();
    event->accept();
}

void NodeEditorView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}

void NodeEditorView::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
}

void NodeEditorView::mousePressEvent(QMouseEvent *event)
{
#if 0
    QtNodes::GraphicsView::mousePressEvent(event);
#else
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        m_clickPos = mapToScene(event->pos());
    }
#endif
}

void NodeEditorView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void NodeEditorView::wheelEvent(QWheelEvent *event) {}

void NodeEditorView::drawBackground(QPainter *painter, const QRectF &r)
{
    QGraphicsView::drawBackground(painter, r);

    if (!m_gridVisible) {
        return;
    }

    auto drawGrid = [&](double gridStep) {
        QRect windowRect = rect();
        QPointF tl = mapToScene(windowRect.topLeft());
        QPointF br = mapToScene(windowRect.bottomRight());

        double left = std::floor(tl.x() / gridStep - 0.5);
        double right = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top = std::floor(br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi) {
            QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi) {
            QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
            painter->drawLine(line);
        }
    };

    auto const &flowViewStyle = QtNodes::StyleCollection::flowViewStyle();

    QPen pfine(flowViewStyle.FineGridColor, 1.0);

    painter->setPen(pfine);
    drawGrid(15);

    QPen p(flowViewStyle.CoarseGridColor, 1.0);

    painter->setPen(p);
    drawGrid(150);
}

void NodeEditorView::paintEvent(QPaintEvent *event)
{
    QtNodes::GraphicsView::paintEvent(event);

    if (m_rubberBandRect.isNull() || m_rubberBandRect.isEmpty()) {
        return;
    }

    QPalette palette = qApp->palette();
    QPainter painter(viewport());
    painter.setPen(QPen(palette.color(QPalette::ColorRole::Accent), 2));
    painter.setBrush(backgroundBrush());
    painter.setOpacity(0.6);
    painter.drawRect(m_rubberBandRect);
    viewport()->update();
}

void NodeEditorView::alignNodesToTop()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal minY = minYOfSelectedNodes();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        position.setY(minY);
        m_model->setNodeData(id, QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::alignNodesToBottom()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal maxY = maxYOfSelectedNodes();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
        position.setY(maxY - size.height());
        m_model->setNodeData(id, QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::alignNodesToLeft()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal minX = minXOfSelectedNodes();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        position.setX(minX);
        m_model->setNodeData(id, QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::alignNodesToRight()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal maxX = maxXOfSelectedNodes();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
        position.setX(maxX - size.width());
        m_model->setNodeData(id, QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::alignNodesToHCenter()
{
    auto nodeIds = m_scene->selectedNodes();
    if (nodeIds.size() < 1) {
        return;
    }

    QtNodes::NodeId id = m_latestSelectedNodeId;
    if (std::find(nodeIds.begin(), nodeIds.end(), m_latestSelectedNodeId) == nodeIds.end()) {
        id = nodeIds.at(0);
    }

    QPointF pos = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
    QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
    qreal hCenter = pos.x() + size.width() / 2;
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
        position.setX(hCenter - size.width() / 2);
        m_model->setNodeData(id, QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::alignNodesToVCenter()
{
    auto nodeIds = m_scene->selectedNodes();
    if (nodeIds.size() < 1) {
        return;
    }

    QtNodes::NodeId id = m_latestSelectedNodeId;
    if (std::find(nodeIds.begin(), nodeIds.end(), m_latestSelectedNodeId) == nodeIds.end()) {
        id = nodeIds.at(0);
    }

    QPointF pos = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
    QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
    qreal vCenter = pos.y() + size.height() / 2;

    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
        position.setY(vCenter - size.height() / 2);
        m_model->setNodeData(id, QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::alignNodesToHEquidistance()
{
    auto nodeIds = orderSelectedNodesByX();
    if (nodeIds.length() < 3) {
        return;
    }

    qreal minX = minXOfSelectedNodes();
    qreal maxX = maxXOfSelectedNodes();
    qreal totalDistance = maxX - minX;
    totalDistance -= m_model->nodeData(nodeIds.last(), QtNodes::NodeRole::Size).toSize().width();
    qreal distance = totalDistance / (nodeIds.length() - 1);
    for (int i = 1; i < nodeIds.length() - 1; i++) {
        QPointF pos = m_model->nodeData(nodeIds[i], QtNodes::NodeRole::Position).toPointF();
        pos.setX(minX + i * distance);
        m_model->setNodeData(nodeIds[i], QtNodes::NodeRole::Position, pos);
    }
}

void NodeEditorView::alignNodesToVEquidistance()
{
    auto nodeIds = orderSelectedNodesByY();
    if (nodeIds.length() < 3) {
        return;
    }

    qreal minY = minYOfSelectedNodes();
    qreal maxY = maxYOfSelectedNodes();
    qreal totalHeight = maxY - minY;
    totalHeight -= m_model->nodeData(nodeIds.last(), QtNodes::NodeRole::Size).toSize().height();
    qreal distance = totalHeight / (nodeIds.length() - 1);
    for (int i = 1; i < nodeIds.length() - 1; i++) {
        QPointF position = m_model->nodeData(nodeIds[i], QtNodes::NodeRole::Position).toPointF();
        position.setY(minY + i * distance);
        m_model->setNodeData(nodeIds[i], QtNodes::NodeRole::Position, position);
    }
}

void NodeEditorView::onRubberBandChanged(QRect viewportRect,
                                         QPointF fromScenePoint,
                                         QPointF toScenePoint)
{
    Q_UNUSED(fromScenePoint);
    Q_UNUSED(toScenePoint);
    m_rubberBandRect = viewportRect;
    update();
}

void NodeEditorView::onNodeDoubleClicked(const QtNodes::NodeId nodeId)
{
    QWidget *w = m_model->nodeData(nodeId, QtNodes::NodeRole::Widget).value<QWidget *>();
    BaseNodeUi *baseNodeUi = qobject_cast<BaseNodeUi *>(w);
    if (!baseNodeUi) {
        return;
    }

    QWidget *popupUi = baseNodeUi->embeddedWidget();
    if (!popupUi) {
        return;
    }

    popupUi->setVisible(!popupUi->isVisible());
    baseNodeUi->adjustSize();
    m_scene->nodeGeometry().recomputeSize(nodeId);
    viewport()->update();
}

void NodeEditorView::onNodeCreated(QtNodes::NodeId nodeId)
{
    QWidget *w = m_model->nodeData(nodeId, QtNodes::NodeRole::Widget).value<QWidget *>();
    BaseNodeUi *baseNodeUi = qobject_cast<BaseNodeUi *>(w);
    if (!baseNodeUi) {
        return;
    }

    QWidget *popupUi = baseNodeUi->embeddedWidget();
    if (!popupUi) {
        return;
    }

    baseNodeUi->adjustSize();
    m_scene->nodeGeometry().recomputeSize(nodeId);

    emit nodeCreated(nodeId);
}

qreal NodeEditorView::minXOfSelectedNodes()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal minX = std::numeric_limits<qreal>::max();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        minX = std::min<qreal>(minX, position.x());
    }
    return minX;
}

qreal NodeEditorView::minYOfSelectedNodes()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal minY = std::numeric_limits<qreal>::max();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        minY = std::min<qreal>(minY, position.y());
    }
    return minY;
}

qreal NodeEditorView::maxXOfSelectedNodes()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal maxX = std::numeric_limits<qreal>::lowest();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
        maxX = std::max<qreal>(maxX, static_cast<qreal>(position.x() + size.width()));
    }
    return maxX;
}

qreal NodeEditorView::maxYOfSelectedNodes()
{
    auto nodeIds = m_scene->selectedNodes();
    qreal maxY = std::numeric_limits<qreal>::lowest();
    for (QtNodes::NodeId id : nodeIds) {
        QPointF position = m_model->nodeData(id, QtNodes::NodeRole::Position).toPointF();
        QSize size = m_model->nodeData(id, QtNodes::NodeRole::Size).toSize();
        maxY = std::max<qreal>(maxY, position.y() + size.height());
    }
    return maxY;
}

QList<QtNodes::NodeId> NodeEditorView::orderSelectedNodesByX()
{
    auto nodeIds = m_scene->selectedNodes();
    // 冒泡排序-增序
    for (int i = 0; i < nodeIds.size(); i++) {
        for (int j = i + 1; j < nodeIds.size(); j++) {
            QPointF pos1 = m_model->nodeData(nodeIds[i], QtNodes::NodeRole::Position).toPointF();
            QPointF pos2 = m_model->nodeData(nodeIds[j], QtNodes::NodeRole::Position).toPointF();
            if (pos1.x() > pos2.x()) {
                std::swap(nodeIds[i], nodeIds[j]);
            }
        }
    }

    QList<QtNodes::NodeId> orderedNodeIds(nodeIds.size());
    for (int i = 0; i < nodeIds.size(); i++) {
        orderedNodeIds[i] = nodeIds[i];
    }
    return orderedNodeIds;
}

QList<QtNodes::NodeId> NodeEditorView::orderSelectedNodesByY()
{
    auto nodeIds = m_scene->selectedNodes();
    // 冒泡排序-增序
    for (int i = 0; i < nodeIds.size(); i++) {
        for (int j = i + 1; j < nodeIds.size(); j++) {
            QPointF pos1 = m_model->nodeData(nodeIds[i], QtNodes::NodeRole::Position).toPointF();
            QPointF pos2 = m_model->nodeData(nodeIds[j], QtNodes::NodeRole::Position).toPointF();
            if (pos1.y() > pos2.y()) {
                std::swap(nodeIds[i], nodeIds[j]);
            }
        }
    }

    QList<QtNodes::NodeId> orderedNodeIds(nodeIds.size());
    for (int i = 0; i < nodeIds.size(); i++) {
        orderedNodeIds[i] = nodeIds[i];
    }
    return orderedNodeIds;
}

QPixmap NodeEditorView::toPixMap(QGraphicsItem *item)
{
#if 0
    auto *node = dynamic_cast<QtNodes::NodeGraphicsObject *>(item);
    if (!node) {
        qWarning() << "Can not convert item to pixmap.";
        return QPixmap();
    }

    QSize size = node->boundingRect().size().toSize();
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.translate(10, 10);
    painter.setRenderHint(QPainter::Antialiasing);
    m_scene->nodePainter().paint(&painter, *node);

    QWidget *w = m_model->nodeData(node->nodeId(), QtNodes::NodeRole::Widget).value<QWidget *>();
    if (w) {
        // 获取系统缩放因子
        qreal scaleFactor = QGuiApplication::primaryScreen()->devicePixelRatio();
        QPixmap pix = w->grab();
        int x = (size.width() - pix.width() / scaleFactor) / 2;
        int y = (size.height() - pix.height() / scaleFactor) / 2;
        painter.drawPixmap(x - 10, y, pix);
    }

    return pixmap;
#else
    QPixmap pixmap(item->boundingRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    m_scene->render(&painter, pixmap.rect(), item->sceneBoundingRect());
    return pixmap;
#endif
}

} // namespace xFlow