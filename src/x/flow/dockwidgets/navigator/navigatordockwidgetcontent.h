/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QGraphicsView>

class NavigatorDockWidgetContent : public QWidget
{
    Q_OBJECT
public:
    explicit NavigatorDockWidgetContent(QScrollArea *scrollArea, QWidget *parent = nullptr);
    ~NavigatorDockWidgetContent() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updateRect();

private:
    QScrollArea *m_scrollArea;
    QRect m_widgetRect;   // QScrollArea::widget()->rect()在此窗口的映射
    QRect m_viewPortRect; // QScrollArea::viewport()->rect()在此窗口的映射
    QPoint m_pressPos;
    bool m_dragging{false};
};
