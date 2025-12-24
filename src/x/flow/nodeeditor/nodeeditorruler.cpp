/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditorruler.h"

#include <QApplication>
#include <QFontMetrics>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QPainter>

#include "nodeeditor.h"

namespace xFlow {

NodeEditorRuler::NodeEditorRuler(QWidget* parent)
    : QWidget(parent)
    , m_editor(nullptr)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    m_vernierColor = palette().color(QPalette::ColorRole::Accent);
#else
    m_vernierColor = palette().color(QPalette::ColorRole::Highlight);
#endif
}

void NodeEditorRuler::setColor(QColor const& color)
{
    m_color = color;
    update();
}

void NodeEditorRuler::setVernierColor(QColor const& color)
{
    m_vernierColor = color;
    update();
}

void NodeEditorRuler::setup(NodeEditor* editor, int direction)
{
    m_editor = editor;
    m_view = qobject_cast<QGraphicsView*>(editor->widget());
    m_direction = direction;
    m_sceneRect = m_view->sceneRect();

    m_editor->installEventFilter(this);
    m_view->installEventFilter(this);
    m_view->viewport()->installEventFilter(this);
    QScrollBar* hBar = m_editor->horizontalScrollBar();
    connect(hBar, &QScrollBar::valueChanged, this, [=]() { updateRuler(); });
    QScrollBar* vBar = m_editor->verticalScrollBar();
    connect(vBar, &QScrollBar::valueChanged, this, [=]() { updateRuler(); });

    updateRuler();
}

bool NodeEditorRuler::showVernier()
{
    return m_setShowVernier;
}

void NodeEditorRuler::setShowVernier(bool show)
{
    m_setShowVernier = show;
}

void NodeEditorRuler::paintEvent(QPaintEvent* event)
{
    if (!m_editor) {
        return;
    }

    QPainter painter(this);
    painter.setPen(QPen(m_color));
    painter.translate(0, 0);
    auto tmp = font();
    tmp.setPixelSize(11);
    painter.setFont(tmp);

    if (m_direction == Qt::Horizontal) {
        paintHorizontal(painter);
    } else if (m_direction == Qt::Vertical) {
        paintVertical(painter);
    }
}

bool NodeEditorRuler::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_view && event->type() == QEvent::Resize) {
        updateRuler();
    } else if (watched == m_view && event->type() == QEvent::Paint) {
        updateRuler();
    } else if (watched == m_editor && event->type() == QEvent::Resize) {
        updateRuler();
    } else if (event->type() == QEvent::MouseMove) {
        if (watched == m_view || watched == m_editor || watched == m_view->viewport()) {
            update();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void NodeEditorRuler::paintHorizontal(QPainter& painter)
{
    int offset = scrollAreaBarValue();
#if 0
    painter.drawLine(QLineF(0, height() - 1, width(), height() - 1));
#endif
    for (int i = 0; i < sceneRect().width(); i += m_tickInterval) {
        int viewPortX = viewPortValue(i);
        viewPortX -= offset;
        if ((i % 150) == 0) {
            QString txt = QString::number(i);
            painter.drawText(viewPortX + 2, height() / 2 + 2, txt);
            painter.drawLine(viewPortX, 0, viewPortX, height());
        } else if ((i % 75) == 0) {
            painter.drawLine(viewPortX, height() - m_longLineLength, viewPortX, height());
        } else {
            painter.drawLine(viewPortX, height() - m_shortLineLength, viewPortX, height());
        }

        if (viewPortX > width()) {
            break;
        }
    }

    if (!isViewContainsMouse() || !m_setShowVernier) {
        return;
    }

    QPoint gPos = m_view->mapFromGlobal(QCursor::pos());
    QPointF scenePos = m_view->mapToScene(gPos);
    int x = viewPortValue(scenePos.x());
    x -= offset;
    painter.setPen(QPen(m_vernierColor));
    painter.drawLine(x, 0, x, height());
}

void NodeEditorRuler::paintVertical(QPainter& painter)
{
    int offset = scrollAreaBarValue();
#if 0
    painter.drawLine(QLineF(width() - 1, 0, width() - 1, height()));
#endif
    for (int i = 0; i < sceneRect().height(); i += m_tickInterval) {
        int viewPortY = viewPortValue(i);
        viewPortY -= offset;
        if ((i % 150) == 0) {
            QString txt = QString::number(i);
            // 画刻度值,要将文本旋转90度
            painter.save();
            painter.translate(2, viewPortY + 4);
            painter.rotate(90);
            painter.drawText(0, 0, txt);
            painter.restore();
            painter.drawLine(0, viewPortY, width(), viewPortY);
        } else if ((i % 75) == 0) {
            painter.drawLine(width() - m_longLineLength, viewPortY, width(), viewPortY);
        } else {
            painter.drawLine(width() - m_shortLineLength, viewPortY, width(), viewPortY);
        }

        if (viewPortY > height()) {
            break;
        }
    }

    if (!isViewContainsMouse() || !m_setShowVernier) {
        return;
    }

    QPoint gPos = m_view->mapFromGlobal(QCursor::pos());
    QPointF scenePos = m_view->mapToScene(gPos);
    int y = viewPortValue(scenePos.y());
    y -= offset;
    painter.setPen(QPen(m_vernierColor));
    painter.drawLine(0, y, width(), y);
}

void NodeEditorRuler::updateRuler()
{
    m_sceneRect = m_view->sceneRect();
    if (m_direction == Qt::Horizontal) {
        setFixedHeight(m_widthOrHeight);
    } else if (m_direction == Qt::Vertical) {
        setFixedWidth(m_widthOrHeight);
    }

    update();
}

int NodeEditorRuler::range()
{
    if (m_direction == Qt::Horizontal) {
        int min = m_editor->horizontalScrollBar()->minimum();
        int max = m_editor->horizontalScrollBar()->maximum();
        return max - min;
    } else {
        int min = m_editor->verticalScrollBar()->minimum();
        int max = m_editor->verticalScrollBar()->maximum();
        return max - min;
    }
}

int NodeEditorRuler::scrollAreaBarValue()
{
    if (m_direction == Qt::Horizontal) {
        return m_editor->horizontalScrollBar()->value();
    } else {
        return m_editor->verticalScrollBar()->value();
    }
}

QRect NodeEditorRuler::sceneRect()
{
    return m_view->sceneRect().toRect();
}

int NodeEditorRuler::sceneValue(int scrollAreaBarValue)
{
    QRect sceneRect = m_view->sceneRect().toRect();
    int range = this->range();

    if (m_direction == Qt::Horizontal) {
        int viewPortWidth = m_view->viewport()->width();
        qreal scale = qreal(viewPortWidth) / qreal(range);
        int viewPortX = scrollAreaBarValue / scale;
        int sceneX = qreal(viewPortX) * qreal(sceneRect.width()) / qreal(viewPortWidth);
        return sceneX;
    } else {
        int viewPortHeight = m_view->viewport()->height();
        qreal scale = qreal(viewPortHeight) / range;
        int viewPortY = scrollAreaBarValue / scale;
        int sceneY = viewPortY * sceneRect.height() / viewPortHeight;
        return sceneY;
    }
}

int NodeEditorRuler::viewPortValue(qreal sceneValue)
{
    if (m_direction == Qt::Horizontal) {
        QRect rect = sceneRect();
        int sceneWidth = rect.width();
        int viewPortWidth = m_view->viewport()->width();
        qreal scale = qreal(viewPortWidth) / qreal(sceneWidth);
        return sceneValue * scale;
    } else {
        QRect rect = sceneRect();
        int sceneHeight = rect.height();
        int viewPortHeight = m_view->viewport()->height();
        qreal scale = qreal(viewPortHeight) / qreal(sceneHeight);
        return sceneValue * scale;
    }
}

bool NodeEditorRuler::isViewContainsMouse()
{
    QPoint pos = m_view->mapFromGlobal(QCursor::pos());
    return m_view->rect().contains(pos);
}

} // namespace xFlow