/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractScrollArea>
#include <QGraphicsView>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>
#include <QScrollBar>

class NodeEditor;
class NodeEditorRuler : public QWidget
{
    Q_OBJECT
public:
    NodeEditorRuler(QWidget* parent = nullptr);
    void setColor(QColor const& color);
    void setVernierColor(QColor const& color);
    void setup(NodeEditor* editor, int direction);

    bool showVernier();
    void setShowVernier(bool show);

signals:
    void sizeChanged(QSize const&);

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void paintHorizontal(QPainter& painter);
    void paintVertical(QPainter& painter);
    void updateRuler();
    int range();
    int scrollAreaBarValue();
    QRect sceneRect();
    int sceneValue(int scrollAreaBarValue);
    int viewPortValue(qreal sceneValue);
    bool isViewContainsMouse();

private:
    NodeEditor* m_editor{nullptr};
    QGraphicsView* m_view{nullptr};
    QRectF m_sceneRect;
    int m_direction;
    int m_widthOrHeight{18};
    int m_tickInterval = 15;   // 每个刻度之间的间隔（像素）
    int m_shortLineLength = 5; // 短线长度
    int m_longLineLength = 10; // 长线长度
    QColor m_color{Qt::gray};
    QColor m_vernierColor{Qt::red};
    bool m_setShowVernier{true};
};
