/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditor.h"

#include <QHBoxLayout>

#include "nodeeditorruler.h"
#include "nodeeditorview.h"

NodeEditor::NodeEditor(NodeEditorRuler *hRuler, NodeEditorRuler *vRuler, QWidget *parent)
    : QScrollArea(parent)
    , m_hRuler(hRuler)
    , m_vRuler(vRuler)
{
    m_view = new NodeEditorView(QColor(Qt::white), this);
    m_view->setScaleRange(1.0, 1.0);
    setContentsMargins(0, 0, 0, 0);
    setWidget(m_view);
}

NodeEditor::~NodeEditor() {}

NodeEditorView *NodeEditor::view() const
{
    return m_view;
}

void NodeEditor::setRulerVisible(bool visible)
{
    m_hRuler->setVisible(visible);
    m_vRuler->setVisible(visible);
}

void NodeEditor::setRulerColor(const QColor &color)
{
    m_hRuler->setColor(color);
    m_vRuler->setColor(color);
}

void NodeEditor::zoomIn()
{
    m_scale += 0.1;

    if (m_scale > maxScale()) {
        m_scale = maxScale();
    }

    setScale(m_scale);
}

void NodeEditor::zoomOut()
{
    m_scale -= 0.1;

    if (m_scale < minScale()) {
        m_scale = minScale();
    }

    setScale(m_scale);
}

qreal NodeEditor::scale() const
{
    return m_scale;
}

void NodeEditor::setScale(qreal scale)
{
    if (scale < minScale()) {
        scale = minScale();
    }

    if (scale > maxScale()) {
        scale = maxScale();
    }

    m_scale = scale;

    static QRectF rect = NodeEditorView::defaultSceneRect();
    auto scene = m_view->scene();
    scene->setSceneRect(rect);
    QSizeF size = rect.size();
    size *= m_scale;
    m_view->setSceneRect(rect);
    m_view->setFixedSize(size.toSize());
    m_view->fitInView(rect);
    emit scaleChanged(scale);
}

qreal NodeEditor::minScale()
{
    return 0.5;
}

qreal NodeEditor::maxScale()
{
    return 2.0;
}
