/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "navigatordockwidgetcontent.h"

#include <QEvent>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollBar>

namespace xFlow {

NavigatorDockWidgetContent::NavigatorDockWidgetContent(QScrollArea *scrollArea, QWidget *parent)
    : m_scrollArea(scrollArea)
{
    setMinimumHeight(168);
    setObjectName("NavigatorDockWidgetContent");
    setStyleSheet("#NavigatorDockWidgetContent {  border: none; }");

    m_scrollArea->installEventFilter(this);
    m_scrollArea->widget()->installEventFilter(this);

    QScrollBar *hBar = m_scrollArea->horizontalScrollBar();
    connect(hBar, &QScrollBar::valueChanged, this, [=]() { update(); });

    QScrollBar *vBar = m_scrollArea->verticalScrollBar();
    connect(vBar, &QScrollBar::valueChanged, this, [=]() { update(); });
}

NavigatorDockWidgetContent::~NavigatorDockWidgetContent() {}

bool NavigatorDockWidgetContent::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_scrollArea || watched == m_scrollArea->widget()) {
        if (event->type() == QEvent::Resize) {
            update();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void NavigatorDockWidgetContent::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    updateRect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPalette palette = qApp->palette();
    QColor backgroundColor = palette.color(QPalette::ColorRole::Base);
    painter.fillRect(event->rect(), backgroundColor);

    if (m_widgetRect.isValid()) {
        QWidget *w = m_scrollArea->widget();
        QGraphicsView *view = qobject_cast<QGraphicsView *>(w);
        if (view && view->scene()) {
            QGraphicsScene *scene = view->scene();
            QPixmap image(w->size());
            image.fill(Qt::transparent);
            QPainter imagePainter(&image);
            scene->render(&imagePainter);
            scene->update();
            painter.drawPixmap(m_widgetRect, image);
        }

        painter.setPen(Qt::darkGray);
        painter.drawRect(m_widgetRect);
    }

    if (m_viewPortRect.isValid()) {
        painter.setPen(palette.color(QPalette::ColorRole::Accent));
        painter.drawRect(m_viewPortRect);
    }
}

void NavigatorDockWidgetContent::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        if (m_viewPortRect.contains(event->pos())) {
            m_pressPos = event->pos();
            m_dragging = true;
            return;
        }
    }

    m_dragging = false;
}

void NavigatorDockWidgetContent::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (event->buttons().testFlag(Qt::LeftButton) && m_dragging) {
        QPoint deltaPos = event->pos() - m_pressPos;
        m_pressPos = event->pos();

        QScrollBar *hBar = m_scrollArea->horizontalScrollBar();
        int min = hBar->minimum();
        int max = hBar->maximum();
        int range = max - min;
        int fitRange = m_widgetRect.width() - m_viewPortRect.width();

        int fitDeltaX = deltaPos.x();
        int value = hBar->value() + fitDeltaX * range / fitRange;
        hBar->setValue(value);

        QScrollBar *vBar = m_scrollArea->verticalScrollBar();
        min = vBar->minimum();
        max = vBar->maximum();
        range = max - min;
        fitRange = m_widgetRect.height() - m_viewPortRect.height();

        int fitDeltaY = deltaPos.y();
        value = vBar->value() + fitDeltaY * range / fitRange;
        vBar->setValue(value);
    }
}

QRect scaleRectToFit(const QRect &rect1, const QRect &rect2)
{
    // 获取rect1和rect2的宽高
    int w1 = rect1.width();
    int h1 = rect1.height();
    int w2 = rect2.width();
    int h2 = rect2.height();

    // 计算宽高比
    double aspectRatio1 = static_cast<double>(w1) / h1;
    double aspectRatio2 = static_cast<double>(w2) / h2;

    // 初始化新的宽高
    int newWidth, newHeight;

    // 根据宽高比调整rect2的尺寸
    if (aspectRatio2 > aspectRatio1) {
        // rect2更宽，调整宽度以适应rect1
        newWidth = w1;
        newHeight = static_cast<int>(w1 / aspectRatio2);
    } else {
        // rect2更高，调整高度以适应rect1
        newHeight = h1;
        newWidth = static_cast<int>(h1 * aspectRatio2);
    }

    // 计算新的rect2的位置，使其居中于rect1
    int newX = rect1.x() + (w1 - newWidth) / 2;
    int newY = rect1.y() + (h1 - newHeight) / 2;

    // 返回新的rect2
    return QRect(newX, newY, newWidth, newHeight);
}

void NavigatorDockWidgetContent::updateRect()
{
    QSize thisSize = this->size();
    QSize widgetSize = m_scrollArea->widget()->size();
    QRect thisRect{0, 0, thisSize.width(), thisSize.height()};
    QRect widgetRect{0, 0, widgetSize.width(), widgetSize.height()};
    m_widgetRect = scaleRectToFit(thisRect, widgetRect);

    /// 计算nav的大小
    int fitWidgetW = m_widgetRect.width();
    int fitWidgetH = m_widgetRect.height();
    int widgetW = widgetSize.width();
    int widgetH = widgetSize.height();

    QSize viewportSize = m_scrollArea->viewport()->size();
    int viewportW = viewportSize.width();
    int viewportH = viewportSize.height();
    int fitViewportW;
    int fitViewportH;

    qreal scaleH = static_cast<double>(fitWidgetH) / widgetH;
    qreal scaleW = static_cast<double>(fitWidgetW) / widgetW;
    fitViewportW = scaleW * viewportW;
    fitViewportH = scaleH * viewportH;

    /// 计算nav的位置
    int minH = m_scrollArea->horizontalScrollBar()->minimum();
    int maxH = m_scrollArea->horizontalScrollBar()->maximum();
    int minV = m_scrollArea->verticalScrollBar()->minimum();
    int maxV = m_scrollArea->verticalScrollBar()->maximum();
    int rangeW = maxH - minH;                  // 水平滚动条的范围
    int rangeH = maxV - minV;                  // 垂直滚动条的范围
    int fitRangeW = fitWidgetW - fitViewportW; // 适应滚动条的范围
    int fitRangeH = fitWidgetH - fitViewportH; // 适应滚动条的范围
    int valueH = m_scrollArea->horizontalScrollBar()->value();
    int valueV = m_scrollArea->verticalScrollBar()->value();

    int x = minH + static_cast<int>(valueH * static_cast<double>(fitRangeW) / rangeW);
    int y = minV + static_cast<int>(valueV * static_cast<double>(fitRangeH) / rangeH);
    x += m_widgetRect.x();
    y += m_widgetRect.y();

    m_viewPortRect = QRect{x, y, fitViewportW, fitViewportH};
}

} // namespace xFlow