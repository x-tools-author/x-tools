/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QScrollArea>

class NodeEditorView;
class NodeEditorRuler;
class NodeEditor : public QScrollArea
{
    Q_OBJECT
public:
    explicit NodeEditor(NodeEditorRuler *hRuler, NodeEditorRuler *vRuler, QWidget *parent = nullptr);
    ~NodeEditor() override;

    NodeEditorView *view() const;
    void setRulerVisible(bool visible);
    void setRulerColor(const QColor &color);

    void zoomIn();
    void zoomOut();
    qreal scale() const;
    void setScale(qreal scale);
    static qreal minScale();
    static qreal maxScale();

signals:
    void scaleChanged(qreal scale);

private:
    NodeEditorView *m_view;
    NodeEditorRuler *m_hRuler;
    NodeEditorRuler *m_vRuler;
    qreal m_scale{1.0};
};
